// CompressionAlgorithmTester.cc
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu
// July 1997
// Modified June 1998

// An abstract class that provides a generic interface for testing
// different compression algorithm.  A subclass of this abstract class
// should act as a wrapper for calls to particular compression and
// decompression algorithms.  Moreover, this class provides the
// functionality required to keep statistics for the results of
// compression tests performed on such subclasses.



#include <stdio.h>
#include <assert.h>
#include "safezlib.hh"
#include "CompressionAlgorithmTester.hh"



// Open an output file whose name is based on the algorithm name
// returned by the getAlgorithmName() private method.
CompressionAlgorithmTester::CompressionAlgorithmTester
(char* outputFilename, const bool initUseFixedImages) :
  useFixedImages (initUseFixedImages) {
  
  outputFile = safegzopen(outputFilename, "w");

  // Output an opening parethesis to begin the list of trace records.
  safegzputs(outputFile, "(\n");

}



CompressionAlgorithmTester::~CompressionAlgorithmTester () {

  // Output a closing parenthesis to end the list of trace records.
  safegzputs(outputFile, ")\n");

  // Close the output file.
  gzclose(outputFile);

}


// Test the speed and efficiency of compression and decompression,
// and output the results to the output file.
void 
CompressionAlgorithmTester::test (TraceRecord* currentRecord,
				  unsigned int uncompressedBytes) {
  // If requested by the user and the given algorithm requires such a
  // thing, use the fixed pages images in which bytes have been
  // reversed for endian differences.
  void* chosenFetchedImage;
  void* chosenEvictedImage;
  if (useFixedImages) {

    chosenFetchedImage = currentRecord->fixedFetchedImage;
    chosenEvictedImage = currentRecord->fixedEvictedImage;

  } else {

    chosenFetchedImage = currentRecord->fetchedImage;
    chosenEvictedImage = currentRecord->evictedImage;

  }
    

  // Note that there may not have been an eviction, and therefore no
  // eviction image on which to perform a test.  If there is no
  // evicted image, just zero out the values to be output.  Otherwise,
  // peform the tests.
  unsigned int compressedSize;
  unsigned long long compressionTime;
  if (currentRecord->evicted) {

    // Call on the private method that actually performs the compression
    // test and returns the measurement results.  Note that the results
    // are returned in the last two arguments.
    performCompressionTest(chosenEvictedImage,
			   uncompressedBytes,
			   compressedSize,
			   compressionTime);

  } else {

    // There was no eviction to test.
    compressedSize = 0;
    compressionTime = 0;

  }

  // Perform timings for decompression of the fetched page.
  unsigned int preDecompressedSize;
  unsigned long long decompressionTime;

  // Call on the private method that actually performs the
  // decompression test and returns the measurement results.  Note
  // that the results are returned in the last two arguments.
  performDecompressionTest(chosenFetchedImage,
			   uncompressedBytes,
			   preDecompressedSize,
			   decompressionTime);

  // Output this information into the results trace.
  outputResults(currentRecord,
		compressedSize,
		compressionTime,
		preDecompressedSize,
		decompressionTime);

}



// This method is called after each test is performed.  It writes to
// the output stream relevant information from the trace record
// (everything but the page image) and the results of these tests.
void
CompressionAlgorithmTester::outputResults
  (TraceRecord* currentRecord,
   unsigned int compressedSize,
   unsigned long long compressionTime,
   unsigned int preDecompressedSize,
   unsigned long long decompressionTime) {

  char buffer[256];

  // Output, in scheme-like list format, a record that contains the
  // same information as the trace record, except that the page images
  // are replaced with information on the compressability and
  // decompressability of those page images.
  safegzputs(outputFile, "  ( ");

  safegzputc(outputFile, (currentRecord->compulsory ? 'C' : 'F'));
  safegzputc(outputFile, ' ');

  safegzputs(outputFile, currentRecord->fetchedPageNumber);
  safegzputc(outputFile, ' ');

  sprintf(buffer, "%d ", preDecompressedSize);
  safegzputs(outputFile, buffer);

  sprintf(buffer, "%lld ", decompressionTime);
  safegzputs(outputFile, buffer);

  if (currentRecord->evicted) {

    safegzputc(outputFile, (currentRecord->dirty ? 'W' : 'R'));
    safegzputc(outputFile, ' ');

    safegzputs(outputFile, currentRecord->evictedPageNumber);
    safegzputc(outputFile, ' ');

  } else {

    safegzputs(outputFile, "N 0 ");

  }

  sprintf(buffer, "%d ", compressedSize);
  safegzputs(outputFile, buffer);

  sprintf(buffer, "%lld ", compressionTime);
  safegzputs(outputFile, buffer);

  // Call on a subclass' virtual method to output additional
  // information to the results file.
  outputAdditionalResults();
  
  safegzputs(outputFile, " )\n");

}
