// WK2Tester.hh
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// October 1997

// A tester designed to work on the Wilson-Kaplan-0
// compression/decompression algorithm.  Note that it note only
// performs the requested compression and decompression, but also
// performs and keeps extra statisics for its own experiements that
// measure characteristics specific to this algorithm.



// Prevent multiple inclusion by surrounding the header file with
// preprocessor directives.
#if !defined (_WK2TESTER_HH)
#define _WK2TESTER_HH


#include "CompressionAlgorithmTester.hh"


class WK2Tester : public CompressionAlgorithmTester {

public:

  // The default constructor.
  WK2Tester (char* outputFilename, const bool initUseFixedImages) :
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



#endif // _WK2TESTER_HH
