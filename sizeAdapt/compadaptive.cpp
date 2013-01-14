// compadaptive.cpp --- a compressed virtual memory simulator that uses recency
//                      information to determine how much of the memory should
//                      be in a compressed state

// CompAdaptive maintains a "history" LRU queue which it uses to tell how often
// pages have been touched at different LRU queue positions. 

// REVIEW: This implementation
//   a) makes a few simplifying assumptions 
//   b) is a little inefficient (doing several linear traversals of the recency
//      data)
// Both (a) and (b) above are a result of choosing the most straightforward
// first approximation of the adaptive compressed memory simulator. Later,
// more efficient (or more accurate) versions can be implemented, but for now
// correctness and implementation speed are crucial

using namespace std;

#include "lruobj.h"
#include "compinput.h"
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <map>

// How many possibilities are there for the ratio of compressed to total memory?
#define NO_BENEFIT       -1
#define NUM_EARLY_POINTS 4
#define PAGE_SIZE        4096
#define forall(iterator) for (int iterator = 0; \
				iterator < NUM_EARLY_POINTS; \
				iterator++)
#define STANDALONE_COMPADAPTIVE -1


// REVIEW: maybe move ASSERT to another file. Ok for now: keep file number small
#ifdef DEBUG
  void _Assert(char *strFile, unsigned uLine) {
    cerr << "\nAssertion failed: " << strFile << ", line " << uLine << endl;
    abort();
  }

  #define ASSERT(f) \
    if (f)          \
        {}          \
    else            \
        _Assert(__FILE__, __LINE__)
#else
  #define ASSERT(f)
#endif

// Record containing the information stored for each block in the queue
struct BlockInfo {
  QueuePos clean_until;
  /* The above value shows how far a dirty page has *previously*
     drifted in the LRU queue. (REVIEW: rename to "clean_until_before" ?)
     This is a non-trivial issue: when a page is touched with a "write"
     reference, it becomes "dirty". The page gets "cleaned" when written back
     to disk. Nevertheless, this simulator keeps track of the cost of 
     implementing many different memory management policies (keeping track of
     the cost is done both online--in our cost-benefit analysis-- and as part
     of the total cost of compressed VM vs. uncompressed). So, a page that is
     clean for some memory size (because it has been written to disk) may be
     dirty for larger memory sizes. This necessitates keeping more than a
     simple "dirty" flag per page: we need to know how far they have gone in
     the recency queue before they got evicted. */
  double compressed_size;
  double compression_time;
  double decompression_time;

  BlockInfo() : clean_until(0), compressed_size(0.0), compression_time(0.0),
    decompression_time(0.0) { }
};

class CompAdaptive;

/* CompressionLruQueue is a variant of LruQueue that cooperates closely with 
   the compressed memory simulator (for instance, updates its statistics while
   traversing the recency structure). Currently a second aspect has been fused
   into the CompressionLruQueue class (REVIEW): it accepts input (touches) in
   a form of page pairs <fetched, evicted> describing the behavior of an
   LRU memory of size filter_size. Then the LRU queue only holds the pages
   beyond the filter_size-most-recently-referenced ones. All this is hidden from
   the adaptive compressed VM simulator (so when class CompAdaptive refers to
   the n-th page in the recency queue, it is indeed the n-th most recently
   referenced page). */
class CompressionLruQueue : public LruQueue { 
  // REVIEW: Is the subtyping really appropriate?
  int filter_size;
  CompAdaptive *simulator;

  QueuePos externalize(QueuePos qpos) {
    return qpos + filter_size;
  }

  QueuePos internalize(QueuePos qpos) {
    return qpos - filter_size;
  }

public:
  int queue_size() {
    return externalize(size);
  }

  bool valid_qpos(QueuePos qpos) {
    return (internalize(qpos) <= max_size);
  }

  // constructor
  CompressionLruQueue(int m_size, int filter, CompAdaptive *sim) :
    LruQueue(m_size-filter), filter_size(filter), simulator(sim) { } 

  LruEvent touch(EventInfo event_info) {
    if (event_info.eviction_tag != 'N') {
      BlockNumber block_num = event_info.fetched_page_number;
      QueueType::iterator i = queue.begin();
      QueuePos qpos = 1;
      // Loop to find the required page
      while (i != queue.end()) {
	if (*i == block_num)
	  break;
	i++;
	qpos++;
      }
      LruEvent temp(0,0,0); // dummy initializer
      if (i == queue.end()) 
	temp = handle_miss(block_num, event_info.evicted_page_number);
      else 
	temp = handle_hit(block_num, event_info.evicted_page_number, qpos, i);
      update_page_information(event_info, temp);
      return temp;
    } 
    // Otherwise the event describes a fetch that does not cause an evict
    // from the filter memory. That is, the memory is being filled up.
    return LruEvent(event_info.fetched_page_number, 0, NOEVICT);
  }

private:
  LruEvent handle_miss(BlockNumber event_fetched, BlockNumber event_evicted) {
    BlockNumber evicted = NOEVICT; 
    if (full())
      evicted = evict(event_fetched);
    // Note what's going on here. Because we don't keep the most recently
    // touched elements in the queue, our first element is the one most
    // recently evicted from the viewpoint of the filter.
    push_node_on_front(event_evicted);
    size++;
    return LruEvent(event_fetched, 0, evicted);
    // A queue position of 0 signifies a miss.
  }

  
  LruEvent handle_hit(BlockNumber event_fetched, BlockNumber event_evicted,
		      QueuePos qpos, QueueType::iterator i) 
  {
    queue.erase(i);
    // Note what's going on here. Because we don't keep the most recently
    // touched elements in the queue, our first element is the one most
    // recently evicted from the viewpoint of the filter.
    push_node_on_front(event_evicted);
    return LruEvent(event_fetched, externalize(qpos), NOEVICT);
  }

  void update_page_information(EventInfo event_info, LruEvent lru_event);

public:

  void uncompress_all_mem();

  void shrink_as_needed();

  void shrink(QueueType::iterator i);

  void compress_n_pages(int n);

  void evict_from_uncompressed();
};


				      
class CompAdaptive {
  friend class CompressionLruQueue;
protected:
  // Definition: replacement interval (r.i.): the time during which
  // a number of "interesting events" (faults, hits in the interesting region)
  // equal to the cache (memory) size occur

  // Definition: virtual time: time measured in "r.i."s

  int mem_size; // size of physical memory
  CompInput input_obj;
  double compressed_ratio[NUM_EARLY_POINTS];         
  // double fractions of mem sz that is compressed
  double stats_decay_factor;/* double decay factor to multiply stats by per 
			      (approx) replacement interval (lower factor
			      decays FASTER) */
  double far_miss_weight;   /* double fraction of a miss that a far miss counts
			      as for advancing time */
  double cost_of_write_to_disk;
  double cost_of_read_from_disk;


  // parameters computed from input parameters
  // stats are decayed about once every eighth of a replacement interval
  QueuePos compression_qpos[NUM_EARLY_POINTS]; 
  // target queue positions where compressed memory should begin at steady state
  // CAUTION: queue positions begin at 1 (i.e.,position 1 holds the MRU element)
  QueuePos earliest_qpos;  // the earliest such position
  QueuePos end_compression_qpos[NUM_EARLY_POINTS];  
  // corresponding queue positions where compressed memory ends (actually,
  // the position right after the end)
  QueuePos latest_qpos;    // the latest such position
  double times_point_reached[NUM_EARLY_POINTS]; 
  // How many times were significant events performed with the compressed
  // memory boundary being at this point?
  QueuePos start_compressed_qpos; // where does the compressed mem. currently 
                                  // begin?
  QueuePos end_compressed_qpos; // where does the compressed mem. end?
  double compressed_space_in_bytes;
  double time_per_fault;    /* virtual time per fault (fraction of replacement
			       interval */
  double decay_per_eighth;  /* decay factor (double between 0 and 1) to multiply
			       stats by per eighth of r.i. */


  /* The values below are averages. They are initialized to an estimate and 
     later updated based on past performance */
  double cost_of_compression;  
  double cost_of_decompression;
  double ratio_of_dirty_evicts;
  double avg_compressed_size;
  /* REVIEW: (approximation caveat) The above variables represent a 
     simplification of the model. We only estimate their values based on the
     current range of compressed memory and over the entire execution of
     the program up to this point, but we use them to estimate how 
     compressing more or less memory would fare. This computation could have
     been made more accurate (e.g., we could actually have one of the above 
     statistics per candidate compressed region of memory, or we could decay
     them in time). For now, the approximation seems reasonable and 
     simplifies implementation. */
  double number_of_compressions; 
  double number_of_decompressions;
  double number_of_fetches;
  double number_of_evicts;

  // basic performance stats
  int compulsory_faults;     // Faults caused by referencing a page for the
                             // first time.
  double lru_execution_time; // time to execute the program without compression
  double execution_time;     // time taken for program execution with compressed
                             // memory

  
  // stats maintained for adaptivity
  double time_before_this_eighth[NUM_EARLY_POINTS]; 
  /* virtual time in (approx) "replacement intervals", not including time so 
     far this 1/8 replacement interval. */
  // These are the stats that get decayed !!!
  double time_this_eighth[NUM_EARLY_POINTS];   // virtual time since last decay
  double early_hits_lately[NUM_EARLY_POINTS];  /* hits to the compressed part
						  that would have also been hits
						  in an uncompressed memory. */
  double late_hits_lately[NUM_EARLY_POINTS];   /* ... the rest. */
  double misses_lately[NUM_EARLY_POINTS];
  CompressionLruQueue* lru_stats_sim;  
  /* lru queue used for determining adaptivity stats,
     and for reflecting current memory state */

  // REVIEW: we are using a separate data structure to hold information for each
  // page. Keeping everything in the recency queue would be a little faster (but
  // would make for a more complicated interface).
  typedef map<BlockNumber, BlockInfo, less<BlockNumber> > InfoMap;
  typedef InfoMap::iterator InfoIterator;
  InfoMap block_info;

public:
  CompAdaptive(int msize,
	       int filter_size,
	       double c_ratio[NUM_EARLY_POINTS],
	       double compressibility_estimate,
	       double write_cost,
	       double read_cost,
	       double compression_cost_estimate,
	       double decompression_cost_estimate,
	       double dirty_ratio_estimate,
	       double decayf,
	       double missweight,
	       CompInput input_object) :
    mem_size(msize),
    input_obj(input_object),
    stats_decay_factor(decayf),
    far_miss_weight(missweight),
    cost_of_write_to_disk(write_cost),
    cost_of_read_from_disk(read_cost), 
    start_compressed_qpos(msize + 1),
    end_compressed_qpos(msize + 1), // i.e., initially, no memory is compressed
    compressed_space_in_bytes(0.0),
    cost_of_compression(compression_cost_estimate),
    cost_of_decompression(decompression_cost_estimate),
    ratio_of_dirty_evicts(dirty_ratio_estimate),
    avg_compressed_size(compressibility_estimate),
    number_of_compressions(0.0),
    number_of_decompressions(0.0),
    number_of_evicts(0.0),
    number_of_fetches(0.0),
    execution_time(0),
    lru_execution_time(0),
    compulsory_faults(0)
  {
    // Initialize input parameters with multiple dimensions
    forall(i) { 
      compressed_ratio[i] = c_ratio[i];
      times_point_reached[i] = 0.0;
      early_hits_lately[i] = 0.0;
      late_hits_lately[i] = 0.0;
      misses_lately[i] = 0.0;
      time_this_eighth[i] = 0.0;
      time_before_this_eighth[i] = 0.0;
    }

    // Initialization of all parameters that are computed from input parameters
    QueuePos num_qpos_compressed[NUM_EARLY_POINTS], 
      eff_qpos_range[NUM_EARLY_POINTS];

    forall(i) {
      num_qpos_compressed[i] = (QueuePos) (mem_size * compressed_ratio[i]);
      eff_qpos_range[i] = (QueuePos) (num_qpos_compressed[i] * 
				      compressibility_estimate); 
      compression_qpos[i] = mem_size - num_qpos_compressed[i] + 1;
      end_compression_qpos[i] = compression_qpos[i] + eff_qpos_range[i];
    }
    earliest_qpos = compression_qpos[0];
    latest_qpos = end_compression_qpos[0];
    forall (i) {
      earliest_qpos = min_qpos(compression_qpos[i], earliest_qpos);
      latest_qpos = max_qpos(end_compression_qpos[i], latest_qpos);
    }
    time_per_fault = 1.0 / mem_size;
    // As a conservative estimate, we only keep track of as many recency
    // positions as would be needed if the entire memory were compressed and
    // yielded compressibility equal to twice the initial (user-supplied) 
    // estimate.
    lru_stats_sim = new 
      CompressionLruQueue(mem_size * 2 * compressibility_estimate, filter_size,
			  this);
    decay_per_eighth = eighth_root(stats_decay_factor);
  }

protected:
    
  QueuePos max_qpos(QueuePos p1, QueuePos p2) { return p1 > p2 ? p1 : p2; }

  QueuePos min_qpos(QueuePos p1, QueuePos p2) { return p1 < p2 ? p1 : p2; }

  double eighth_root(double number) { return sqrt(sqrt(sqrt(number))); }
  
  void maybe_decay_stats() {
    forall (i)
      while (time_this_eighth[i] > 0.125) // more than 1/8 repl. interval?
	decay_stats(i);
    /* do it again in case there's more than 1/8 repl. interval left
       since after decaying once.  This should only happen for very
       small queue sizes (around 8 or less), where a single fault
       can advance time by more than 1/8 of a replacement interval. */
  }

  virtual void decay_stats(int compression_index) {
    // add 1/8 replacement interval to time before this eighth
    time_before_this_eighth[compression_index] += 0.125;
    // subtract 1/8 replacement interval from time for current eighth
    // (we're really starting a new eighth, but keeping the extra
    // left over from the previous one)
    time_this_eighth[compression_index] -= 0.125;
    
    // decay the counts of early and late faults by the factor
    // appropriate to an eighth of a replacement interval
    early_hits_lately[compression_index] *= decay_per_eighth;
    late_hits_lately[compression_index] *= decay_per_eighth;
    misses_lately[compression_index] *= decay_per_eighth;
  }
  
  // estimate the benefit if we were to compress as much memory as the
  // compression_index indicates
  double benefit(int compression_index) {
    return late_hits_lately[compression_index] * cost_of_read_from_disk +
      late_hits_lately [compression_index] * ratio_of_dirty_evicts * 
      cost_of_write_to_disk;
    // REVIEW: note the above approximation of the actual number of writebacks
    // avoided.
  }

  // estimate the cost if we were to compress as much memory as the
  // compression_index indicates
  double cost (int compression_index) {
    return (late_hits_lately[compression_index] + 
	    early_hits_lately[compression_index] + 
	    misses_lately[compression_index])*
      (cost_of_compression + cost_of_decompression);
  }

  int ideal_compression_index() {
    double max_difference = 0.0;
    int compression_index = 0;
    forall (i) {
      double difference = benefit(i) - cost(i);
      if (difference > max_difference) {
	max_difference = difference;
	compression_index = i;
      }
    }
    if (max_difference > 0.0)
      return compression_index;
    return NO_BENEFIT;
  }

  QueuePos ideal_compression_qpos() {
    int ideal_index = ideal_compression_index();
    if (ideal_index == NO_BENEFIT)
      return mem_size + 1;
    return compression_qpos[ideal_index];
  }

  // The second argument indicates whether the page evicted was 
  // compressed (which is the case if there are any compressed pages)
  void simulate_evict(BlockNumber block, bool compressed) {
    BlockInfo &info = block_info[block];
    double dirty_evicts;
    if (info.clean_until < end_compressed_qpos) {  // dirty page
      execution_time += cost_of_write_to_disk;
      dirty_evicts = number_of_evicts * ratio_of_dirty_evicts + 1;
    } else
      dirty_evicts = number_of_evicts * ratio_of_dirty_evicts;
    number_of_evicts++;
    ratio_of_dirty_evicts = dirty_evicts / number_of_evicts;
    if (compressed) {
      end_compressed_qpos--;
      compressed_space_in_bytes -= info.compressed_size;
    }
  }
  
  void simulate_compress(BlockNumber block) {
    BlockInfo &info = block_info[block];

    execution_time += info.compression_time;
    // Update statistics (averages) as a result of compressing one more page
    double total_compression_time = cost_of_compression *
      number_of_compressions + info.compression_time;
    double total_compressed_bytes = avg_compressed_size *
      number_of_compressions + info.compressed_size;
    number_of_compressions++;
    cost_of_compression = total_compression_time / number_of_compressions;
    avg_compressed_size = total_compressed_bytes / number_of_compressions;
    start_compressed_qpos--;
    compressed_space_in_bytes += info.compressed_size;
  }

  // The second argument indicates whether the page to be uncompressed was
  // fetched from disk or just from compressed memory
  void simulate_uncompress(BlockNumber block, bool fetch) {
    BlockInfo &info = block_info[block];
    execution_time += info.decompression_time;
    double total_decompression_time = cost_of_decompression * 
      number_of_decompressions + info.decompression_time;

    number_of_decompressions++;
    cost_of_decompression = total_decompression_time / number_of_decompressions;
    start_compressed_qpos++;
    if (fetch) { 
      execution_time += cost_of_read_from_disk;
      number_of_fetches++;
      end_compressed_qpos++;
      // The end of the compressed memory should never be beyond the recency
      // position tracked in our queue. REVIEW: should this be an ASSERT or not?
      if (!lru_stats_sim->valid_qpos(end_compressed_qpos - 1 )) {
	cerr << "ERROR: compressed region exceeded recency queue" << endl;
	exit(1);
      }
    }
    else
      compressed_space_in_bytes -= info.compressed_size;
  }

  void simulate_compulsory_fault() {
    start_compressed_qpos++;
    end_compressed_qpos++;
    // The end of the compressed memory should never be beyond the recency
    // position tracked in our queue. REVIEW: should this be an ASSERT or not?
    if (!lru_stats_sim->valid_qpos(end_compressed_qpos - 1 )) {
      cerr << "ERROR: compressed region exceeded recency queue" << endl;
      exit(1);
    }
  }

  void simulate_uncompressed_fetch(BlockNumber block) {
    lru_execution_time += cost_of_read_from_disk;
  }

  void simulate_uncompressed_evict(BlockNumber block) {
    if (block_info[block].clean_until <= mem_size) 
      lru_execution_time += cost_of_write_to_disk;
  }


public:
  // REVIEW:
  // This routine is not optimized for efficiency. For instance, we go and
  // traverse the recency queue multiple times (probably each time a method
  // of "lru_stats_sim" is invoked).
  void touch(EventInfo event_info) {
    QueuePos ideal_qpos = ideal_compression_qpos();
    bool grow = (ideal_qpos < start_compressed_qpos), 
      shrink = (start_compressed_qpos < ideal_qpos);
    LruEvent lru_event = lru_stats_sim->touch(event_info);
    QueuePos lru_qpos = lru_event.queue_position();

    // Invariant A: 
    //  earliest_qpos <= start_compressed_qpos <= mem_size + 1 <= 
    //  end_compressed_qpos
    ASSERT (earliest_qpos <= start_compressed_qpos &&
	    start_compressed_qpos <= mem_size + 1 &&
	    mem_size + 1 <= end_compressed_qpos);

    // MAIN COMPRESSED VM ADAPTIVITY LOGIC
    if (lru_qpos >= start_compressed_qpos || lru_qpos == 0) {
      // See "IMPORTANT", below.
      if (event_info.compulsory)
	simulate_compulsory_fault();
      else
	simulate_uncompress(event_info.fetched_page_number, // miss?
			    (lru_qpos >=end_compressed_qpos || lru_qpos == 0));
      // At this point, invariant A may not hold, but it will be restored
      if (grow) {
	lru_stats_sim->compress_n_pages(1); 
	/* 1 page may be enough, esp. if the hit is in compressed mem. */
	if (compute_bytes_needed() > 0)  
	  lru_stats_sim->compress_n_pages(1);
      }
      else if (!shrink && (start_compressed_qpos != end_compressed_qpos)) {
	// Note that there may be NO compressed pages!
	lru_stats_sim->compress_n_pages(1);
	update_ideal_size_reached();  // Just statistics--not part of main logic
      }
      lru_stats_sim->shrink_as_needed();
    }
    // END MAIN COMPRESSED VM ADAPTIVITY LOGIC

    /* IMPORTANT:
       Note that we first perform the uncompression and possible fetch, and
       then adjust the compressed memory to fit everything. This is the
       opposite order from what a real compressed VM system would do, but
       it doesn't matter for the simulation. The reason we do this is that
       it's easier to keep our conceptual model: the moment a page is touched
       and moved to the front of the queue, we uncompress so that the indexes
       pointing to the compressed recency region are up-to-date. An
       alternative would be to break the recency operations into two: first
       see which page was touched and which page would be evicted, then
       adjust the memory, then update the recency info (i.e., actually move
       the page to the front of the queue). This will require a reorganization
       of class CompressionLruQueue */ 

    // Record cost of uncompressed memory for comparison purposes
    if (lru_qpos > mem_size || lru_qpos == 0) {
      if (eviction_needed())
	lru_stats_sim->evict_from_uncompressed();
      if (!event_info.compulsory)  // if it's not a compulsory fault
	simulate_uncompressed_fetch(event_info.fetched_page_number);
    }

    if (event_info.compulsory)
      compulsory_faults++;
    else 
      record_histogram_event(lru_qpos);
    maybe_decay_stats();

    // If we have some compressibility data to work with, update the stats.
    if (mem_size - 2 /* arbitrary */ > start_compressed_qpos)
      update_compression_regions();
  }

protected:
  bool eviction_needed() {
    return (lru_stats_sim->queue_size() > mem_size);
  }

  // update the statistics showing how many times a compression candidate size
  // was reached.
  void update_ideal_size_reached() {
    forall (i)
      if (start_compressed_qpos == compression_qpos[i]) {
	times_point_reached[i]++;
	break;
      }
  }
  
  double compute_bytes_needed() {
    return compressed_space_in_bytes - 
      (mem_size + 1 - start_compressed_qpos) * PAGE_SIZE;
  }

  // update boundaries for candidate compression regions using new stats
  void update_compression_regions() {
    double compressibility = 
      (end_compressed_qpos - start_compressed_qpos) * PAGE_SIZE /
      compressed_space_in_bytes;
    forall(i) {
      QueuePos num_qpos_compressed = mem_size - compression_qpos[i] + 1;
      end_compression_qpos[i] = (QueuePos) 
	(compression_qpos[i] + (num_qpos_compressed * compressibility));
    }
  }

  /* recording an LRU event.  note that this updates virtual time
     as well as recording the significance of the particular event
     in terms of queue position hits in a pure lru queue */
  void record_histogram_event(QueuePos qpos) {
    if (qpos == 0 || qpos >= latest_qpos) 
      record_far_lru_miss(qpos);
    else if (qpos < earliest_qpos)
      record_easy_lru_hit(qpos);
    else record_interesting_lru_hit(qpos);
  }

  /* what should this do?  Should it update virtual time?  note that
     when we have a lot of far misses, it may decrease the reliability
     of our heuristic, because reference patterns will slide up the
     LRU queue (e.g., "blowing smoke" or "wheatfield in wind" pattern)
     Maybe eventually we should try a heuristic that guesses that the
     current patterns will move down the queue when there are a lot
     of far misses... i.e., use statistics from further up the queue.
     Or just back off to LRU?

     It's arguable that this shouldn't increment time at all (because
     it's "out of range", timescale-relativity wise.)  It's also arguable
     that it should have some special effect because it may indirectly
     affect the validity of the normal heuristic. */
  void record_far_lru_miss(QueuePos qpos) {
    forall(i)
      time_this_eighth[i] += far_miss_weight * time_per_fault;
  }

  void record_easy_lru_hit(QueuePos qpos) {
  }  // currently does nothing---should it do anything?

  // record a hit in the interesting range as either early (favoring LRU)
  // or late (favoring early eviction)
  void record_interesting_lru_hit(QueuePos qpos) {
#ifdef SAME_TIMESCALE
    forall(i)
      time_this_eighth[i] += time_per_fault;
#endif      
    if (qpos >= mem_size)
      record_late_hit(qpos);
    else record_early_hit(qpos);
  }
  
  void record_late_hit(QueuePos qpos) {
    forall (i) 
      if (qpos < end_compression_qpos[i]) {
	late_hits_lately[i]++;
#ifndef SAME_TIMESCALE
	time_this_eighth[i] += time_per_fault;
#endif
      } 
#ifndef SAME_TIMESCALE
      else  
	// if it's not a late hit, it's a far miss. Time should still be updated
	time_this_eighth[i] += far_miss_weight * time_per_fault;
#endif
  }
      
  void record_early_hit(QueuePos qpos) {
    forall (i)
      if (qpos >= compression_qpos[i]) {
	early_hits_lately[i]++;
#ifndef SAME_TIMESCALE
	time_this_eighth[i] += time_per_fault;
#endif
      }
  }

public:
  void disp() {
    cout << "COMPRESSED VM SIMULATOR STATE:" << endl;
    
    cout << "mem_size: " << mem_size << endl;
    
    forall (i) {
      cout << "compression_qpos[" << i << "]: " << compression_qpos[i] << 
	"   end_compression_qpos[" << i << "]: " << end_compression_qpos[i] << 
	endl;
    }

    forall (i) {
      cout << "times_point_reached[" << i << "]: " << times_point_reached[i]
	   << endl;
    }

    cout << "start_compressed_qpos: " << start_compressed_qpos << 
      "   end_compressed_qpos: " << end_compressed_qpos << 
      endl;
    
    cout << "lru_execution_time: " << lru_execution_time << 
      "    compressed_VM_execution_time: " << execution_time
	 << endl;
    
    forall (i) {
      cout << "time before this eighth[" << i << "]: " << 
	time_before_this_eighth[i] 
	   << "   time this eighth[" << i << "]: " << 
	time_this_eighth[i] << endl;
    }

    forall (i) {
      cout << "early_hits_lately[" << i << "]: " << early_hits_lately[i]
	   << "  late_hits_lately[" << i << "]: " << late_hits_lately[i]
	   << endl;
    }

    cout << "number of compressions: " << number_of_compressions << endl;
    cout << "number of decompressions: " << number_of_decompressions << endl;
    cout << "number of evicts: " << number_of_evicts << endl;

    cout << "ratio of dirty evicts: " << ratio_of_dirty_evicts << endl;
    cout << "avg. cost of compression: " << cost_of_compression << endl;
    cout << "avg. cost of decompression: " << cost_of_decompression << endl;

    // REVIEW: add more interesting statistics if needed (e.g., how many times
    // a certain compression point was chosen)
  }

public:
  // "simulate" reads from input
  void simulate() {
    do {
      EventInfo event = input_obj.get_next();
      if (input_obj.end_of_input())
	break;
      touch(event);
    } while (1);
  }

  void list_output () {
    cout << "(" << endl <<
      " (lru_execution_time " << lru_execution_time << 
      ")" << endl <<
      " (compressed_VM_execution_time " << execution_time << 
      ")" << endl <<
      " (number_of_compressions " << number_of_compressions << 
      ")" << endl <<
      " (number_of_decompressions " << number_of_decompressions << 
      ")" << endl <<
      " (avg_compression_cost " << cost_of_compression << 
      ")" << endl <<
      " (avg_decompression_cost " << cost_of_decompression << 
      ")" << endl <<
      " (total_compression_cost " << number_of_compressions * 
          cost_of_compression << 
      ")" << endl <<
      " (total_decompression_cost " << number_of_decompressions * 
          cost_of_decompression << 
      ")" << endl <<
      " (avg_compressed_size " << avg_compressed_size << 
      ")" << endl <<
      " (number_of_faults " << number_of_fetches << 
      ")" << endl <<
      " (number_of_compulsory_faults " << compulsory_faults << 
      ")" << endl <<
      " (number_of_evicts " << number_of_evicts << 
      ")" << endl <<
      " (number_of_dirty_evicts " << number_of_evicts * ratio_of_dirty_evicts <<
      ")" << endl <<
      " (total_fault_cost " << number_of_fetches * cost_of_read_from_disk << 
      ")" << endl <<
      " (total_dirty_evict_cost " << number_of_evicts * ratio_of_dirty_evicts *
          cost_of_write_to_disk << 
      ")" << endl ;
    forall (i) {
      cout << 
	" (times_point_reached_" << i << " " << times_point_reached[i] <<
	")" << endl;
    }
    cout << ")" << endl;
  }

  // Output the estimated execution time for compressed VM or plain LRU
  void output (ostream& LRU_stream,
	       ostream& comp_vm_stream) {
    LRU_stream << lru_execution_time << endl;
    comp_vm_stream << execution_time << endl;
  }  
};


// IMPLEMENTATION OF METHODS IN CLASS CompressionLruQueue

// REVIEW (secondary): we are being a little wasteful in this routine,
// passing structures around when not all information is needed.
void CompressionLruQueue::update_page_information(EventInfo event_info, 
						  LruEvent lru_event) {
  // Step 1: Update dirtyness and all other information for the page that is 
  // about to enter the recency queue (i.e., the page evicted from filter).
  BlockInfo &info = simulator->block_info[event_info.evicted_page_number];
  ASSERT(event_info.evicted_compressed_size != 0);
  info.compressed_size = event_info.evicted_compressed_size;
  info.compression_time = event_info.compression_time;
  if (event_info.eviction_tag == 'W')
    info.clean_until = externalize(1);
  else if (info.clean_until == 0) 
    // REVIEW (indirect): indicates that the value was never set
    info.clean_until = externalize(max_size + 1);
  
  // Step 2: Update page information for the page being fetched.
  BlockInfo &info_fetched = 
    simulator->block_info[event_info.fetched_page_number];
  QueuePos &clean_until_for_page_touched = info_fetched.clean_until;
  QueuePos lru_qpos = lru_event.queue_position();
  if (lru_qpos == 0)
    // if the page was evicted from the recency queue, mark it clean for
    // all memory sizes.
    clean_until_for_page_touched = externalize(max_size + 1);
  else 
    // mark the point to which the touched page has drifted (clean_until).
    // Recall that we designate a clean page with a large clean_until value.
    clean_until_for_page_touched = 
      simulator->max_qpos(lru_qpos, // externalized position! 
			  clean_until_for_page_touched);
  
#ifdef DEBUG
  // Make sure that the compressed size reported at decompression is the same
  // as that at compression
  if (info_fetched.compressed_size != 0) 
    if (info_fetched.compressed_size != event_info.fetched_compressed_size) {
      cerr << "ERROR: " << info_fetched.compressed_size << " instead of " <<
	event_info.fetched_compressed_size << " for page: " <<
	event_info.fetched_page_number << endl;
      abort();
    }
#endif

  info_fetched.decompression_time = event_info.decompression_time; 
}


void CompressionLruQueue::uncompress_all_mem() {
  // SPECIAL CASE: compression doesn't seem to pay off. 
  // Uncompress all memory!
  QueuePos first_compressed_qpos =
    internalize(simulator->start_compressed_qpos);
  QueueType::iterator i = block_in_pos(first_compressed_qpos);
  do {
    simulator->simulate_uncompress(*i, false);
    i++;
  } while (simulator->start_compressed_qpos !=simulator->end_compressed_qpos);  
}

void CompressionLruQueue::shrink_as_needed() {

#ifdef DEBUG
  {
  // DEBUG
  QueuePos first_compressed_qpos =
    internalize(simulator->start_compressed_qpos);
  QueueType::iterator i = block_in_pos(first_compressed_qpos);
  QueueType::iterator j = 
    block_in_pos(internalize(simulator->end_compressed_qpos));
  double sum = 0;
  while (i != j) {
    sum += simulator->block_info[*i].compressed_size;
    i++;
  } 
  if (sum != simulator->compressed_space_in_bytes) {
    QueueType::iterator i = block_in_pos(first_compressed_qpos - 1);
    QueueType::iterator j = 
      block_in_pos(internalize(simulator->end_compressed_qpos));
    cerr << "< ";
    while (i != j) {
      cerr << simulator->block_info[*i].compressed_size << "[" << *i << "] ";
      i++;
    }
    cerr << simulator->block_info[*i].compressed_size << "[" << *i << "] ";
    // Note that we print the element before the beginning of the compressed
    // region and after its end as well.
    cerr << "> " << sum << " " << simulator->compressed_space_in_bytes << endl;
    simulator->disp(); // HACK
    abort();
  } 
    
  }
#endif

  QueuePos last_compressed_qpos = 
    internalize(simulator->end_compressed_qpos) - 1;
  QueueType::iterator i = block_in_pos(last_compressed_qpos);
  while (simulator->compute_bytes_needed() > 0) {
    shrink(i);
    i--;
  }
}

// This routine captures all the interesting boundary conditions cleanly.
// There are many ways to shrink the compressed memory region.
// NOTE: this routine depends on only being called when certain conditions are
// true (realistically, it will only be called from "shrink_as_needed").
void CompressionLruQueue::shrink(QueueType::iterator last_block) {
  // Note that the condition below means that no pages are compressed
  if (simulator->end_compressed_qpos == simulator->start_compressed_qpos) {
    // Note what this means: no pages are compressed, but we still need to
    // shrink the memory. That is, an uncompressed eviction may be needed.
    // That is: end_compressed_qpos == start_compressed_qpos == mem_size + 2.
    ASSERT(simulator->start_compressed_qpos == simulator->mem_size + 2);
    if (simulator->eviction_needed()) {
      last_block--; // Consequently, the iterator was off by 1.
      simulator->simulate_evict(*last_block, false);
    }
    simulator->start_compressed_qpos = simulator->end_compressed_qpos = 
      simulator->mem_size + 1; 
    // normalize the representation of an empty compressed memory.
  } 
  else if (simulator->end_compressed_qpos == simulator->mem_size + 1) 
    uncompress_all_mem();
  else 
    simulator->simulate_evict(*last_block, true);
}

void CompressionLruQueue::compress_n_pages(int n) {
  QueuePos last_uncompressed_qpos = 
    internalize(simulator->start_compressed_qpos) - 1;
  QueueType::iterator i = block_in_pos(last_uncompressed_qpos);
  while (n > 0) {
    simulator->simulate_compress(*i);
    i--;
    n--;
  }
}

void CompressionLruQueue::evict_from_uncompressed() {
  QueuePos uncompress_evict_qpos = 
    internalize(simulator->mem_size) + 1; 
  // the page that would have been evicted from an uncompressed memory is the
  // one after the memory limit (recency queue has been updated when this
  // method is called).
  QueueType::iterator i = block_in_pos(uncompress_evict_qpos);
  simulator->simulate_uncompressed_evict(*i);
}



#ifdef STANDALONE_COMPADAPTIVE
// For testing

int main(int argc, char** argv) {
  
  CompInput my_in(&cin); //the address of the input stream

  if (argc != 11 + NUM_EARLY_POINTS) {
    cerr << "usage: " << argv[0] << 
      " <memory size> <filter size> <compression candidate points (" << 
      NUM_EARLY_POINTS << ")> <compressibility estimate>" << 
      " <write cost> <read cost> <compression cost estimate>" <<
      " <decompression cost estimate> <dirty ratio estimate>" <<
      " <decay_factor> <far_miss_weight>" << endl;
    exit(1);
  }

  int msize;
  int filter_size;
  double c_ratio[NUM_EARLY_POINTS];
  double compressibility_estimate;
  double write_cost;
  double read_cost;
  double compression_cost_estimate;
  double decompression_cost_estimate;
  double dirty_ratio_estimate;
  double decayf;
  double missweight;
  (istringstream)(argv[1]) >> msize;
  (istringstream)(argv[2]) >> filter_size;  
  forall (i) 
    (istringstream)(argv[3+i]) >> c_ratio[i];
  (istringstream)(argv[3+NUM_EARLY_POINTS]) >> compressibility_estimate; 
  (istringstream)(argv[4+NUM_EARLY_POINTS]) >> write_cost; 
  (istringstream)(argv[5+NUM_EARLY_POINTS]) >> read_cost; 
  (istringstream)(argv[6+NUM_EARLY_POINTS]) >> compression_cost_estimate; 
  (istringstream)(argv[7+NUM_EARLY_POINTS]) >> decompression_cost_estimate; 
  (istringstream)(argv[8+NUM_EARLY_POINTS]) >> dirty_ratio_estimate; 
  (istringstream)(argv[9+NUM_EARLY_POINTS]) >> decayf; 
  (istringstream)(argv[10+NUM_EARLY_POINTS]) >> missweight; 

  CompAdaptive q(msize, filter_size, c_ratio, compressibility_estimate, 
		 write_cost, read_cost, compression_cost_estimate, 
		 decompression_cost_estimate, dirty_ratio_estimate, decayf,
		 missweight, my_in);
  q.simulate();
  //  q.disp();
  q.list_output();
}
#endif
