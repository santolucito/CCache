// test-compression.cc
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// August 1997
// Modified June 1998

// This program accepts an LRU-fault-filtered trace of page images.
// (Each record contains fetch and eviction page images, representing
// the traffic into and out of a memory of some given size.) Each
// fetch image is tested for decompression speed and effectiveness,
// and each eviction image is tested for compression speed and
// effectiveness.

// Updated June 1998
// The zlib library is used.  The page-image traces being read into
// this utility are normally quite large.  If the decompression is
// performed as a separate process, then context switching may
// interfere with accurate timings of the compressions and
// decompressions.  By using this library, the decompression of the
// souce trace is performed within this program, and is thus
// serialized with respect to the timings.

using namespace std;

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include "zlib.h"
#include "safezlib.hh"
#include "CompressionAlgorithmTester.hh"
#include "TraceRecord.hh"



// This function determines which compression/decompression algorithms
// will be tested by this program.  In order to add an algorithm, it
// must first be wrapped in a tester (see the
// CompressionAlgorithmTester class).  Then, include the header file
// for that tester, and add a line of code to the function below that
// inserts a pointer to a new tester object onto the end of the vector
// of testers.  That's it!
#include "WK2Tester.hh"
#include "WKdmTester.hh"
#include "LZOTester.hh"
#include "LZRW1Tester.hh"
//#include "WKdmWithLZOTester.hh"
//#include "WKdmIntoLZOTester.hh"
//#include "WKdm-24-8Tester.hh"
void
initializeTesterVector
(vector<CompressionAlgorithmTester*>& initVector,
 const bool fixEndianness) {
    
  // Add any desired testers here.
  initVector.push_back(new WK2Tester("WK2.results.gz", fixEndianness));
  initVector.push_back(new WKdmTester("WKdm.results.gz", fixEndianness)); 
  initVector.push_back(new LZOTester("LZO.results.gz", fixEndianness));
  initVector.push_back(new LZRW1Tester("LZRW1.results.gz",
  				       fixEndianness));

}



// Delete each tester used, allowing them to perform cleanup before
// execution ends.
void
cleanTesterVector
  (vector<CompressionAlgorithmTester*>& cleanVector) {

  for (vector<CompressionAlgorithmTester*>::iterator cleanIterator =
	 cleanVector.begin();
       cleanIterator != cleanVector.end();
       ++cleanIterator) {
    delete *(cleanIterator);
  }
}



void
processTrace (gzFile inputFile,
	      const unsigned int tracePageSize,
	      const bool fixEndianness,
	      const unsigned int recordsAtATime) {

  // Create and initialize the vector of testers, each of which
  // provides a consistent interface to test each desired compression
  // algorithm.
  vector<CompressionAlgorithmTester*> testerVector;
  initializeTesterVector(testerVector, fixEndianness);
  cout << recordsAtATime;
  // Create a vector of trace records.
  TraceRecord* records[recordsAtATime];
  for (unsigned int recordNumber = 0;
       recordNumber < recordsAtATime;
       recordNumber++) {

    records[recordNumber] = new TraceRecord(tracePageSize);

  }
  // Read through the entire trace, and let each algorithm compress
  // and decompress each evicted and fetched page.
  bool done = false;
  while (!done) {
    // Read as many records as requested (or as many as are left in
    // the trace).
    unsigned int recordsRead = 0;
    while ((!done) && (recordsRead < recordsAtATime)) {

      // Note: To avoid dependance on the EOF flag, which is
      // inconsistently set depending on function and platform, we
      // perform a check for an explicit end-of-trace character, 'Q'.
      // It's an ugly workaround, but it is reliable.
      char testChar = safegzgetc(inputFile);

      if (testChar == 'Q') {

	done = true;

      } else {

	// Read the next trace record.
	records[recordsRead]->read(inputFile, testChar);
	recordsRead++;

      }
    }

    // Loop through each tester, passing the batch of records for
    // compression and decompression testing.
    for (vector<CompressionAlgorithmTester*>::iterator testerIterator
	   = testerVector.begin();
	 testerIterator != testerVector.end();
	 testerIterator++) {

      for (unsigned int recordsTimed = 0;
	   recordsTimed < recordsRead;
	   recordsTimed++) {
	
        // Test the compression and decompression and output the
	// results.
	(*testerIterator)->test(records[recordsTimed],
				tracePageSize);
      } // Records-at-a-time loop
    } // CompressionAlgorithmTester loop
  } // Trace reading loop

  // Clean up the vector of testers.
  cleanTesterVector(testerVector);

  cout<<"check1";
  // Deallocate the trace records.
  for (unsigned int recordNumber = 0;
       recordNumber < recordsAtATime;
       recordNumber++) {

    delete records[recordNumber];

  }
}


// A global variable.  Ugly, but easy.
unsigned int processorCyclesPerSecond;
int
main (int argc, char** argv) {

  if (argc != 6) {
    cerr << "Usage: "
	 << argv[0]
	 << " <input filename>"
	 << " <trace page size>"
	 << " <fix endianness?>"
	 << " <processor speed in MHz>"
	 << " <cache size in kilobytes>"
	 << endl;
    exit(-1);
  }

  // Give names to the arguments.
  char* inputPathname = argv[1];
  const unsigned int tracePageSize = atoi(argv[2]);
  const bool fixEndianness = (argv[3][0] == 'y');
  const unsigned int processorCyclesPerSecond = atoi(argv[4]) * 1000000;
  const unsigned int cacheSize = atoi(argv[5]);

  // Use the compressed cache size to determine how many records to
  // read at a time to avoid caching.  We choose to read as many
  // records as there are K in the cache, since it is a simple measure
  // that, at 8K per record, should be a large enough chunk of data to
  // prevent the cache from artificially improving results.
  unsigned int recordsAtATime = cacheSize;
  
  // Open the input and output files.
  gzFile inputFile = safegzopen(inputPathname, "r");
  
  // Initiate the trace processing loop.
  processTrace(inputFile,
	       tracePageSize,
	       fixEndianness,
	       recordsAtATime);
  
  // Close the input file.
  gzclose(inputFile);
  cout << "\nshit actually got to the end!";
  return 0;
}
