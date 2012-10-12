// WKdmTester.cc
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// October 1997

// A tester designed to work on the Wilson-Kaplan-0
// compression/decompression algorithm.  Note that it note only
// performs the requested compression and decompression, but also
// performs and keeps extra statisics for its own experiements that
// measure characteristics specific to this algorithm.


#include <iostream>
#include "WKdmTester.hh"
#include "WKdm.h"


const unsigned int bytesPerWK_word = sizeof(WK_word) / sizeof(char);


// Perform the actual task of timing the compression algorithm.
void
WKdmTester::performCompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnCompressedSize,
   unsigned long long& returnCompressionTime) {

  WK_word compressionBuffer[4 * uncompressedBytes / bytesPerWK_word];

  // Surround a call to the compression routine with a timing
  // mechanism.
  //START_TIMER;

  //this is an attempt to replace the timer
  //start
  struct timespec start, stop;
  long accum;

  clock_gettime(CLOCK_REALTIME, &start);
  //endstart

  returnCompressedSize =
    WKdm_compress((WK_word*)uncompressedData,
		  compressionBuffer,
		  uncompressedBytes / bytesPerWK_word);

  //stop
  clock_gettime(CLOCK_REALTIME, &stop);

  accum = (stop.tv_sec - start.tv_sec)
        + (stop.tv_nsec - start.tv_nsec);
        /// 1000000000L;

  returnCompressionTime = accum;
  //endstop
  // Set the value that will return the compression time.
  //STOP_TIMER(returnCompressionTime);

}



// Perform the actual task of timing the decompression algorithm.
void
WKdmTester::performDecompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnPreDecompressionSize,
   unsigned long long& returnDecompressionTime) {  

  returnPreDecompressionSize = 0;
  returnDecompressionTime = 0;


  unsigned int uncompressedWords = uncompressedBytes / bytesPerWK_word;
  WK_word compressionBuffer[4 * uncompressedWords];
  WK_word decompressionBuffer[uncompressedWords];

  // Get a compressed copy of the page so that the decompression can
  // be timed.
  returnPreDecompressionSize =
    WKdm_compress((WK_word*)uncompressedData,
		  compressionBuffer,
		  uncompressedWords);

  // Surround a call to the decompression routine with a timing
  // mechanism.
  //START_TIMER;

  //start
  struct timespec start, stop;
  long accum;

  clock_gettime(CLOCK_REALTIME, &start);
  //endstart

  WKdm_decompress(compressionBuffer,
		  decompressionBuffer,
		  uncompressedWords);
   //stop
  clock_gettime(CLOCK_REALTIME, &stop);

  accum = (stop.tv_sec - start.tv_sec)
        + (stop.tv_nsec - start.tv_nsec);
        /// 1000000000.0;

  returnDecompressionTime = accum;

  //endstop 

  // Set the value that will return the decompression time.
  //STOP_TIMER(returnDecompressionTime);
  
}
