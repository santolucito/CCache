#ifndef LRUOBJ_H
#define LRUOBJ_H

#include "general.h"
#include "input.h"
#include <iostream>
//#include <algo.c>

#define LOWER_LIMIT -2
#define NOEVICT -3

typedef int QueuePos;

class LruEvent {
  BlockNumber block_num;
  QueuePos queue_pos;
  BlockNumber evicted;
  
public:
  LruEvent(BlockNumber b_n, QueuePos q_p, BlockNumber ev) : block_num(b_n), 
    queue_pos(q_p), evicted(ev) { }

  QueuePos queue_position() { return queue_pos; }
  BlockNumber evicted_block() { return evicted; }
  void disp() {
    cout << "#[<lru-event> " << "block-num: " << block_num << 
      " queue-pos: " << queue_pos << " evicted: " << evicted << "]" << endl;
  }
};


class LruQueue {
protected:
  int size;
  Input input_obj;

  typedef list<BlockNumber> QueueType;
  QueueType queue;

public:
  int max_size;
  // constructor
  LruQueue(int m_size, Input input_object) : 
    max_size(m_size), size(0), input_obj(input_object), queue() { } 
  LruQueue(int m_size) :
    max_size(m_size), size(0), input_obj(&cin), queue() { } 

  bool empty() { return (size == 0); }
  bool full() { return (size == max_size); }
  void push_node_on_front(BlockNumber block_num) 
  { queue.push_front(block_num); }


  virtual LruEvent touch (BlockNumber block_num) {
    QueueType::iterator i = queue.begin();
    QueuePos qpos = 1;
    // Loop to find the required page
    while (i != queue.end()) {
      if (*i == block_num)
	break;
      i++;
      qpos++;
    }
    if (i == queue.end()) 
      return handle_miss(block_num);
    else 
      return handle_hit(block_num, qpos, i);
  }

  // returns the set of all blocks that are less recently touched than "from"
  // (which must be in the queue)
  SetOfBlocks& blocks_after (BlockNumber from) {
    SetOfBlocks& result = *(new SetOfBlocks);
    if (from != LOWER_LIMIT) {
      QueueType::iterator i;
      i = ++(find(queue.begin(), queue.end(), from));
      while (i != queue.end())
	result.insert(*i++);
    }
    return result;
  }

  // Was "recent" touched more (or equally) recently than "old"?
  bool more_recent (BlockNumber recent, BlockNumber old) {
    if (recent != LOWER_LIMIT) {
      QueueType::iterator i;
      i = find(queue.begin(), queue.end(), recent);
      if (old == LOWER_LIMIT)
	return true;
      while (i != queue.end())
	if (*i++ == old)
	  return true;
    }
    return false;
  }

  // This is useful for algorithms that check if the last page in an RLY
  // LRU queue is in memory (e.g., the Wood, Fernandez, and Lang algorithm).
  BlockNumber lru_block() {
    return queue.back();
  }

  // Return the position of a block. Again, this is useful for
  // sophisticated variants of our eviction algorithm that base decisions on
  // the presence in memory of blocks other than the one currently being
  // touched.
  QueuePos block_position(BlockNumber block_num) {
    QueuePos count = 1;
    for (QueueType::iterator i = queue.begin(); i != queue.end(); i++, count++)
      if (*i == block_num)
	break;
    return count;
  }

protected:  
  virtual BlockNumber evict(BlockNumber block_num) { return evict_lru_block(); }

  LruEvent handle_miss(BlockNumber block_num) {
    BlockNumber evicted = NOEVICT; 
    if (full())
      evicted = evict(block_num);
    push_node_on_front(block_num);
    size++;
    return LruEvent(block_num, 0, evicted);
    // A queue position of 0 signifies a miss.
  }
  
  LruEvent handle_hit(BlockNumber block_num, QueuePos qpos, 
		      QueueType::iterator i) 
  {
    if (qpos != 1) {
      queue.erase(i);
      push_node_on_front(block_num);
    }
    return LruEvent(block_num, qpos, NOEVICT);
  }

  BlockNumber evict_lru_block() {
    BlockNumber block_num = *(--queue.end());
    queue.pop_back();
    size--;
    return block_num;
  }

  QueueType::iterator block_in_pos(QueuePos mru_pos) {
    QueueType::iterator i = queue.begin();
    QueuePos qpos = 1;
    // Loop to find the required page
    while (i != queue.end()) {
      if (qpos == mru_pos)
	break;
      i++;
      qpos++;
    }
    return i;
  }

  BlockNumber evict_nth_mru_block(QueuePos mru_pos) {
    QueueType::iterator i = block_in_pos(mru_pos);
    BlockNumber block_num = *i;
    queue.erase(i);
    size--;
    return block_num;
  }

  // Simulation methods
public:
  list<BlockNumber> list_of_block_nums() { return queue; }

  virtual void disp() { }

  // "simulate" reads from input
  void simulate() {
    do {
      BlockNumber block = input_obj.get_next();
      if (input_obj.end_of_input())
	break;
      touch(block);
    } while (1);
  }

  // "sim" can be used with small inputs for testing purposes (reads from an
  // in-memory input source and produces rich output).
  void sim(list<BlockNumber> trace) {
    list<BlockNumber>::iterator i = trace.begin();
    do {
      cout << "***in sim***" << endl;
      disp();
      if (i != trace.end()) {
	LruEvent event = touch(*i);
	event.disp();
      }
      else {
	cout << "Simulation finished." << endl;
	break;
      }
      i++;
    } while (1);
  }

};

BlockNumber st0[] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
BlockNumber st1[] = {10, 5, 10, 5, 10, 5, 7, 10, 5, 7};
BlockNumber st2[] = {1, 5, 1, 5, 6, 5, 1, 5, 1, 5, 7, 8, 9, 8, 7, 1, 5, 1, 5, 
		     7, 8, 6, 5, 1, 22, 20, 21, 8, 5, 6, 7, 8, 9, 1, 5, 1, 5, 
		     6, 5, 1, 5, 1, 22, 20, 21, 19, 17, 16, 8, 9, 8, 7, 1, 5, 
		     17, 5, 6, 5, 17, 18, 1, 6, 7, 8, 9, 10, 11, 12, 8, 9, 10, 
		     11, 12, 1, 5, 1, 6, 5, 8, 9, 10, 11, 12, 13, 14, 15, 16, 
		     11, 12, 13, 14, 15, 16, 15, 1, 6, 5, 8, 9, 10, 21, 20, 22,
		     8, 5, 6, 7, 8, 9};

list<BlockNumber> l0 (st0, st0 + 12);
list<BlockNumber> l1 (st1, st1 + 10);
list<BlockNumber> l2 (st2, st2 +  sizeof(st2)/sizeof(BlockNumber));

#endif

#ifdef STANDALONE_LRU

void main() {
  LruQueue* q;
  q = new LruQueue(5);
  q->sim(l0);
  delete(q);
  q = new LruQueue(5);
  q->sim(l1);
  delete(q);
  q = new LruQueue(5);
  q->sim(l2);
  delete(q);
}

#endif


