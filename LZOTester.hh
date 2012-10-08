// LZOTester.hh
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// October 1997

// A tester designed to work on the Lempel-Ziv-Oberhumer
// compression/decompression algorithm.  Note that we've chosen to use
// the LZO1f variant--according to the results provided by Oberhumer,
// that variant does best with short compression sources.



// Prevent multiple inclusion by surrounding the header file with
// preprocessor directives.
#if !defined (_LZOTESTER_HH)
#define _LZOTESTER_HH


#include "CompressionAlgorithmTester.hh"


class LZOTester : public CompressionAlgorithmTester {

public:

  // The default constructor
  LZOTester (char* outputFilename, const bool initUseFixedImages) :
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



#endif // _LZOTESTER_HH
