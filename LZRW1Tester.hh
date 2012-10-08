// LZRW1Tester.hh
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// August 1997

// A tester designed to work on the Lempel-Ziv-R?-Williams-1
// compression/decompression algorithm.  We've chosen to test this
// particular algorithm because Douglis used it in his paper on
// compressed RAM caches.



// Prevent multiple inclusion by surrounding the header file with
// preprocessor directives.
#if !defined (_LZRW1TESTER_HH)
#define _LZRW1TESTER_HH


#include "CompressionAlgorithmTester.hh"


class LZRW1Tester : public CompressionAlgorithmTester {

public:

  // The default constructor.
  LZRW1Tester (char* outputFilename, const bool initUseFixedImages) :
    CompressionAlgorithmTester(outputFilename, false) {}

protected:

  // Perform the actual tasks of timing the compression and
  // decompression of a given algorithm.
  virtual void performCompressionTest
    (void* uncompressedData,
     unsigned int uncompressedBytes,
     unsigned int& returnCompressedSize,
     unsigned long long& returnCompressionTime);
  virtual void performDecompressionTest
    (void* uncompressedData,
     unsigned int uncompressedBytes,
     unsigned int& returnPreDecompressionSize,
     unsigned long long& returnDecompressionTime);

};



#endif // _LZRW1TESTER_HH
