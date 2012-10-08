// CompressionAlgorithmTester.hh
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// October 1997

// An abstract class that provides a generic interface for testing
// different compression algorithm.  A subclass of this abstract class
// should act as a wrapper for calls to particular compression and
// decompression algorithms.  Moreover, this class provides the
// functionality required to keep statistics for the results of
// compression tests performed on such subclasses.

using namespace std;

// Prevent multiple inclusion by surrounding the header file with
// preprocessor directives.
#if !defined (_COMPRESSIONALGORITHMTESTER_HH)
#define _COMPRESSIONALGORITHMTESTER_HH



#include <cstring>
#include "zlib.h"
#include "TraceRecord.hh"

// Macros that will allow subclasses to turn on and off the correct
// timer, and declare a proper timer variable, for the current
// platform.  These are implemented as macros and not methods to avoid
// the extra time required for procedure calls, as these timings are
// using very high resolution timers.

#if defined (__linux__)

//#include "ZenTimer.hh"

#define START_TIMER \
  zen_timer timer; \
  timer.timer_on()

extern unsigned int processorCyclesPerSecond;
const unsigned int nanosecondsPerSecond = 1000000000;
#define STOP_TIMER(timingResult) \
  timer.timer_off(); \
  timingResult = (timer.getTime() / processorCyclesPerSecond * nanosecondsPerSecond)

#elif defined (__svr4__)

#include<sys/time.h>

#define START_TIMER \
  hrtime_t startTime; \
  hrtime_t stopTime; \
  startTime = gethrvtime()

#define STOP_TIMER(timingResult) \
  stopTime = gethrvtime(); \
  timingResult = stopTime - startTime;

#endif



class CompressionAlgorithmTester {

public:

  // Open an output file whose name is based on the algorithm name
  // returned by the getAlgorithmName() private method.
  CompressionAlgorithmTester (char* outputFilename,
			      const bool initUseFixedImages);

  ~CompressionAlgorithmTester ();

  // Test the speed and efficiency of compression and decompression,
  // and output the results to the output file.
  void test (TraceRecord* currentRecord,
	     unsigned int uncompressedBytes);

protected:

  // The compressed output file to which test results are written as a
  // trace.
  gzFile outputFile;

  // A flag that indicates whether or not the fixed endianness verion
  // of each page image needs to be used.
  bool useFixedImages;

  // Perform the actual tasks of timing the compression and
  // decompression of a given algorithm.  (The methods above really
  // just collect statistics, and rely on these protected methods to
  // perform the actual tests.)  These methods need to be specified in
  // the subclass.
  virtual void performCompressionTest
    (void* uncompressedData,
     unsigned int uncompressedBytes,
     unsigned int& returnCompressedSize,
     unsigned long long& returnCompressionTime) = 0;
  virtual void performDecompressionTest
    (void* uncompressedData,
     unsigned int uncompressedBytes,
     unsigned int& returnPreDecompressionSize,
     unsigned long long& returnDecompressionTime) = 0;

  // This method is called after each test is performed.  It writes to
  // the output stream relevant information from the trace record
  // (everything but the page image) and the results of these tests.
  // It also calls outputAdditionalResults, in case a particular
  // subclass wants to output additional information in each trace
  // record.
  void outputResults (TraceRecord* currentRecord,
		      unsigned int compressedSize,
		      unsigned long long compressionTime,
		      unsigned int preDecompressedSize,
		      unsigned long long decompressionTime);

  // This virtual method can be refined on a per-subclass basis.  Any
  // subclass can redefine this method so that extra information is
  // output for that particular algorithm.  By default, this method
  // does nothing.
  void outputAdditionalResults () {}

};

#endif // _COMPRESSIONALGORITHMTESTER_HH
