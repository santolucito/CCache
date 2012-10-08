// WK2Tester.cc
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// August 1997

// A tester designed to work on the Wilson-Kaplan-0
// compression/decompression algorithm.  Note that it note only
// performs the requested compression and decompression, but also
// performs and keeps extra statisics for its own experiements that
// measure characteristics specific to this algorithm.

using namespace std;

#include <iostream>
#include <cstring>
#include "WK2Tester.hh"
#include "WK2.h"


const unsigned int bytesPerWord = sizeof(int) / sizeof(char);


// Perform the actual task of timing the compression algorithm.
void
WK2Tester::performCompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnCompressedSize,
   unsigned long long& returnCompressionTime) {

  WK_word compressionBuffer[4 * uncompressedBytes / bytesPerWord];

  // Surround a call to the compression routine with a timing
  // mechanism.
  //START_TIMER;
  returnCompressedSize =
    WK2_compress((WK_word*)uncompressedData,
		 compressionBuffer,
		 uncompressedBytes / bytesPerWord);
  //STOP_TIMER(returnCompressionTime);

}



// Perform the actual task of timing the decompression algorithm.
void
WK2Tester::performDecompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnPreDecompressionSize,
   unsigned long long& returnDecompressionTime) {

  unsigned int uncompressedWords = uncompressedBytes / bytesPerWord;
  WK_word compressionBuffer[4 * uncompressedWords];
  WK_word decompressionBuffer[uncompressedWords];

  // Get a compressed copy of the page so that the decompression can
  // be timed.
  returnPreDecompressionSize =
    WK2_compress((WK_word*)uncompressedData,
		 compressionBuffer,
		 uncompressedWords);

  // Surround a call to the decompression routine with a timing
  // mechanism.
  //START_TIMER;
  WK2_decompress(compressionBuffer,
		 decompressionBuffer,
		 uncompressedWords);
  //STOP_TIMER(returnDecompressionTime);

}
