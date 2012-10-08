// WKdmWithLZOTester.cc
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// May 1998

// A tester designed to work on the Wilson-Kaplan-dm algorithm that
// re-compressed its misses with LZO, a fast Lempel-Ziv implementation
// Note that it note only performs the requested compression and
// decompression, but also performs and keeps extra statisics for its
// own experiements that measure characteristics specific to this
// algorithm.

using namespace std;

#include <iostream>
#include <cstring>
#include "WKdmWithLZOTester.hh"
#include "WKdm-with-LZO.h"


const unsigned int bytesPerWord = sizeof(int) / sizeof(char);


// Perform the actual task of timing the compression algorithm.
void
WKdmWithLZOTester::performCompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnCompressedSize,
   unsigned long long& returnCompressionTime) {

  WK_word compressionBuffer[4 * uncompressedBytes / bytesPerWord];

  // Surround a call to the compression routine with a timing
  // mechanism.
  START_TIMER;
  returnCompressedSize =
    WKdm_with_LZO_compress((WK_word*)uncompressedData,
			   compressionBuffer,
			   uncompressedBytes);
  STOP_TIMER(returnCompressionTime);

}



// Perform the actual task of timing the decompression algorithm.
void
WKdmWithLZOTester::performDecompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnPreDecompressionSize,
   unsigned long long& returnDecompressionTime) {

  returnPreDecompressionSize = 0;
  returnDecompressionTime = 0;

  unsigned int uncompressedWords =
    uncompressedBytes / sizeof(WK_word);
  WK_word compressionBuffer[4 * uncompressedWords];
  WK_word decompressionBuffer[uncompressedWords];

  // Get a compressed copy of the page so that the decompression can
  // be timed.
  returnPreDecompressionSize =
    WKdm_with_LZO_compress((WK_word*)uncompressedData,
			   compressionBuffer,
			   uncompressedBytes);

  // Surround a call to the decompression routine with a timing
  // mechanism.
  START_TIMER;
  WKdm_with_LZO_decompress(compressionBuffer,
			    decompressionBuffer);
  STOP_TIMER(returnDecompressionTime);
  
}
