// LZRW1Tester.cc
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// August 1997

// A tester designed to work on the Lempel-Ziv-R?-Williams-1
// compression/decompression algorithm.  We've chosen to test this
// particular algorithm because Douglis used it in his paper on
// compressed RAM caches.

using namespace std;

#include <iostream>
#include <stdlib.h>
#include "LZRW1Tester.hh"
#include "lzrw1.h"



// Perform the actual task of timing the compression algorithm.
void
LZRW1Tester::performCompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnCompressedSize,
   unsigned long long& returnCompressionTime) {

  UBYTE compressionBuffer[2 * uncompressedBytes];
  ULONG compressedSize;

  // Surround a call to the compression routine with a timing
  // mechanism.
  //START_TIMER;

  //this is an attempt to replace the timer
  //start
  struct timespec start, stop;
  long accum;

  clock_gettime(CLOCK_REALTIME, &start);
  //endstart

  lzrw1_compress((UBYTE*)uncompressedData,
		 (ULONG)uncompressedBytes,
		 compressionBuffer,
		 &compressedSize);

  //stop
  clock_gettime(CLOCK_REALTIME, &stop);

  accum = (stop.tv_sec - start.tv_sec)
        + (stop.tv_nsec - start.tv_nsec);
        /// 1000000000L;

  returnCompressionTime = accum;
  //endstop

  //STOP_TIMER(returnCompressionTime);

  

  // Set the return values.
  returnCompressedSize = compressedSize;

}



// Perform the actual task of timing the decompression algorithm.
void
LZRW1Tester::performDecompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnPreDecompressionSize,
   unsigned long long& returnDecompressionTime) {

  UBYTE compressionBuffer[2 * uncompressedBytes];
  UBYTE decompressionBuffer[uncompressedBytes];
  ULONG compressedSize;
  ULONG decompressedSize;

  // Get a compressed copy of the page so that the decompression can
  // be timed.
  lzrw1_compress((UBYTE*)uncompressedData,
		 (ULONG)uncompressedBytes,
		 compressionBuffer,
		 &compressedSize);

  // Surround a call to the decompression routine with a timing
  // mechanism.
  //START_TIMER;

//start
  struct timespec start, stop;
  long accum;

  clock_gettime(CLOCK_REALTIME, &start);
  //endstart

  lzrw1_decompress(compressionBuffer,
		   compressedSize,
		   decompressionBuffer,
		   &decompressedSize);

  //stop
  clock_gettime(CLOCK_REALTIME, &stop);

  accum = (stop.tv_sec - start.tv_sec)
        + (stop.tv_nsec - start.tv_nsec);
        /// 1000000000.0;

  returnDecompressionTime = accum;

  //endstop 

  //STOP_TIMER(returnDecompressionTime);

  // A small sanity check.
  if (decompressedSize != uncompressedBytes) {
    cerr << "LZRW1Tester::performDecompressionTest: "
	 << "decompression size doesn't match original"
	 << endl;
    exit(-1);
  }

  // Set the return values.
  returnPreDecompressionSize = compressedSize;

}
