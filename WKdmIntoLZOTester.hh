// WKdmIntoLZOTester.hh
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// May 1998

// A tester designed to work on the Wilson-Kaplan-dm algorithm that
// re-compressed its misses with LZO, a fast Lempel-Ziv implementation
// Note that it note only performs the requested compression and
// decompression, but also performs and keeps extra statisics for its
// own experiements that measure characteristics specific to this
// algorithm.



// Prevent multiple inclusion by surrounding the header file with
// preprocessor directives.
#if !defined (WKdmINTOLZOTESTER_HH)
#define WKdmINTOLZOTESTER_HH

#include "CompressionAlgorithmTester.hh"

class WKdmIntoLZOTester : public CompressionAlgorithmTester {

public:

  // The default constructor.
  WKdmIntoLZOTester (char* outputFilename, const bool initUseFixedImages) :
    CompressionAlgorithmTester(outputFilename, initUseFixedImages) {}

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



#endif // WKdmINTOLZOTESTER_HH
