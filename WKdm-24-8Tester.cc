// WKdm_24_8Tester.cc
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// October 1998

// A tester designed to work on the Wilson-Kaplan-0
// compression/decompression algorithm.  Note that it note only
// performs the requested compression and decompression, but also
// performs and keeps extra statisics for its own experiements that
// measure characteristics specific to this algorithm.

using namespace std;

#include <iostream>
#include "WKdm-24-8Tester.hh"
#include "WKdm-24-8.h"


const unsigned int bytesPerWKdm_24_8_word = (sizeof(WKdm_24_8_word) /
					     sizeof(char));


// Perform the actual task of timing the compression algorithm.
void
WKdm_24_8Tester::performCompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnCompressedSize,
   unsigned long long& returnCompressionTime) {

  WKdm_24_8_word compressionBuffer[4 * uncompressedBytes /
				  bytesPerWKdm_24_8_word];

  // Surround a call to the compression routine with a timing
  // mechanism.
  START_TIMER;
  returnCompressedSize =
    WKdm_24_8_compress((WKdm_24_8_word*)uncompressedData,
		       compressionBuffer,
		       uncompressedBytes / bytesPerWKdm_24_8_word);

  // Set the value that will return the compression time.
  STOP_TIMER(returnCompressionTime);

}



// Perform the actual task of timing the decompression algorithm.
void
WKdm_24_8Tester::performDecompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnPreDecompressionSize,
   unsigned long long& returnDecompressionTime) {  

  returnPreDecompressionSize = 0;
  returnDecompressionTime = 0;


  unsigned int uncompressedWords =
    uncompressedBytes / bytesPerWKdm_24_8_word;
  WKdm_24_8_word compressionBuffer[4 * uncompressedWords];
  WKdm_24_8_word decompressionBuffer[uncompressedWords];

  // Get a compressed copy of the page so that the decompression can
  // be timed.
  returnPreDecompressionSize =
    WKdm_24_8_compress((WKdm_24_8_word*)uncompressedData,
		       compressionBuffer,
		       uncompressedWords);

  // Surround a call to the decompression routine with a timing
  // mechanism.
  START_TIMER;
  WKdm_24_8_decompress(compressionBuffer,
		       decompressionBuffer,
		       uncompressedWords);

  // Set the value that will return the decompression time.
  STOP_TIMER(returnDecompressionTime);
  
}
