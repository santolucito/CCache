// LZOTester.cc
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// August 1997

// A tester designed to work on the Lempel-Ziv-Oberhumer
// compression/decompression algorithm.  Note that we've chosen to use
// the LZO1f variant--according to the results provided by Oberhumer,
// that variant does best with short compression sources.

using namespace std;

#include <iostream>
#include <stdlib.h>
#include "LZOTester.hh"
#include "lzo1f.h"
#include <time.h>


// Perform the actual task of timing the compression algorithm.
void
LZOTester::performCompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnCompressedSize,
   unsigned long long& returnCompressionTime) {

  lzo_byte compressionBuffer[2 * uncompressedBytes];
  lzo_byte _workingMemory[LZO1F_MEM_COMPRESS + 16];
  lzo_uint compressedSize = sizeof(compressionBuffer);
  int resultCode;

  // Initialize the working memory.
  lzo_byte* workingMemory = (lzo_byte*)LZO_ALIGN(_workingMemory, 16);

  // Surround a call to the compression routine with a timing
  // mechanism.
  //START_MYTIMER;

  //start
  struct timespec start, stop;
  long accum;

  clock_gettime(CLOCK_REALTIME, &start);
  //endstart

  resultCode =
    lzo1f_1_compress((lzo_byte*)uncompressedData,
		     uncompressedBytes,
		     compressionBuffer,
		     &compressedSize,
		     workingMemory);

  //stop
  clock_gettime(CLOCK_REALTIME, &stop);

  accum = (stop.tv_sec - start.tv_sec)
        + (stop.tv_nsec - start.tv_nsec);
        /// 1000000000.0;

  returnCompressionTime = accum;

  //endstop  

  //STOP_MYTIMER;

  // Ensure that the compression was successful.
  if (resultCode != LZO_E_OK) {
    cerr << "LZO::performCompressionTest: "
	 << "Compression failed = "
	 << resultCode
	 << endl;
    exit(-1);
  }

  // Set the return values.
  returnCompressedSize = compressedSize;

}



// Perform the actual task of timing the decompression algorithm.
void
LZOTester::performDecompressionTest
  (void* uncompressedData,
   unsigned int uncompressedBytes,
   unsigned int& returnPreDecompressionSize,
   unsigned long long& returnDecompressionTime) {

  lzo_byte compressionBuffer[2 * uncompressedBytes];
  lzo_byte decompressionBuffer[uncompressedBytes];
  lzo_byte _workingMemory[LZO1F_MEM_COMPRESS + 16];
  lzo_uint compressedSize;
  lzo_uint decompressedSize;
  int resultCode;

  // Initialize the working memory.
  lzo_byte* workingMemory = (lzo_byte*)LZO_ALIGN(_workingMemory, 16);

  // Get a compressed copy of the page so that the decompression can
  // be timed.
  resultCode =
    lzo1f_1_compress((lzo_byte*)uncompressedData,
		     uncompressedBytes,
		     compressionBuffer,
		     &compressedSize,
		     workingMemory);

  // Ensure that the compression was successful.
  if (resultCode != LZO_E_OK) {
    cerr << "LZO::performDecompressionTest: "
	 << "Compression failed = "
	 << resultCode
	 << endl;
    exit(-1);
  }

  // Surround a call to the decompression routine with a timing
  // mechanism.
  //START_TIMER;

//start
  struct timespec start, stop;
  long accum;

  clock_gettime(CLOCK_REALTIME, &start);
  //endstart

  resultCode = lzo1f_decompress(compressionBuffer,
				compressedSize,
				decompressionBuffer,
				&decompressedSize,
				0);

  //stop
  clock_gettime(CLOCK_REALTIME, &stop);

  accum = (stop.tv_sec - start.tv_sec)
        + (stop.tv_nsec - start.tv_nsec);
        /// 1000000000.0;

  returnDecompressionTime = accum;

  //endstop 

 // STOP_TIMER(returnDecompressionTime);

  // Ensure that the decompression was successful.
  if (resultCode != LZO_E_OK) {
    cerr << "LZO::performDecompressionTest: "
	 << "Decompression failed = "
	 << resultCode
	 << endl;
    exit(-1);
  }

  // Set the return values.
  returnPreDecompressionSize = compressedSize;

}
