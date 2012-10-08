#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include "TraceRecord.hh"
#include "safezlib.hh"

using namespace std;

// During construction, we need only copy the size of the pages to be
// read, and allocate enough space to store those page images.
TraceRecord::TraceRecord (unsigned int initTracePageSize) :
  tracePageSize (initTracePageSize)
{
  
  // Allocate space for the page images.
  fetchedImage = malloc(tracePageSize);
  if (fetchedImage == 0) {
    perror("TraceRecord::TraceRecord allocating fetched image");
  }

  evictedImage = malloc(tracePageSize);
  if (evictedImage == 0) {
    perror("TraceRecord::TraceRecord allocating evicted image");
  }

  fixedFetchedImage = malloc(tracePageSize);
  if (fixedFetchedImage == 0) {
    perror("TraceRecord::TraceRecord allocating fixed fetched image");
  }

  fixedEvictedImage = malloc(tracePageSize);
  if (fixedEvictedImage == 0) {
    perror("TraceRecord::TraceRecord allocating fixed evicted image");
  }

}



TraceRecord::~TraceRecord () {

  delete fetchedImage;
  delete evictedImage;
  delete fixedFetchedImage;
  delete fixedEvictedImage;

}


// Given a open trace file, read in one record from that trace.  (It
// is assumed that the trace format is the LRU-filtered-page-dump
// format, in which each record holds the fetched and, if dirtied, the
// evicted image of that page from the tracer's unprotected state.)
// Return, whether or not the record was for a compulsory fault, the
// faulted paged number, the fault image, whether or not the page was
// dirtied, the evicted page number, and (if dirtied) the evicted
// image.
void
TraceRecord::read (gzFile inputFile, char tag) {

  // Parse the compulsory field.
  char compulsoryField = tag;
  switch (compulsoryField) {
  case 'C':
    compulsory = true;
    break;
  case 'F':
    compulsory = false;
    break;
  default:
    cerr << "Invalid compulsory field: " << compulsoryField << endl;
    exit(-1);
  }

  // Read the page number.
  safegzread(inputFile, fetchedPageNumber, 4);
  fetchedPageNumber[4] = '\0';

  // Read the faulted page image.
  readPageImage(inputFile, fetchedImage, fixedFetchedImage);

  // Read in the dirty flag and page number.  The former will indicate
  // whether or not there is a page image to be read.
  char dirtyField = safegzgetc(inputFile);
  safegzread(inputFile, evictedPageNumber, 4);
  evictedPageNumber[4] = '\0';

  switch (dirtyField) {
  case 'W':
    evicted = true;
    dirty = true;
    break;
  case 'R':
    evicted = true;
    dirty = false;
    break;
  case 'N':
    //cout << "check point"; 
    evicted = false;
    // A small sanity check--the page number should always be zero if
    // there was no eviction in this record.
    if (evictedPageNumber[0] != '0' &&
	evictedPageNumber[1] != '0' &&
	evictedPageNumber[2] != '0' &&
	evictedPageNumber[3] != '0') {
      cerr << "Invalid page number "
	   << evictedPageNumber
	   << " for empty eviction section"
	   << endl;
      exit(-1);
    }
    break;
  default:
    cerr << "Invalid dirty field: " << (int)dirtyField << endl;
    exit(-1);
  }

  // If appropriate, read the evicted page image.  Note that even if
  // the page wasn't dirtied, we still need to read the image so that
  // the file position marker is properly maintained.
  if (evicted) {
    readPageImage(inputFile, evictedImage, fixedEvictedImage);
  }
}



// Perform the reading of a page image.  Store a copy of the page read
// and that same page with the endianness reversed.  For some
// algorithms, the user may need that algorithm to compress the fixed
// image.
void TraceRecord::readPageImage (gzFile inputFile,
				 void* returnImage,
				 void* fixedReturnImage) {

  // Read the page image as a sequence of bytes into the provided
  // image space.
  safegzread(inputFile, returnImage, tracePageSize);

  // Create the fixed image by reversing the order of every 4 bytes.
  char* original = (char*)returnImage;
  //string mystring = original;
  //cout << mystring;
  char* fixed = (char*)fixedReturnImage;
  for (unsigned int baseByte = 0;
       baseByte < tracePageSize;
       baseByte += 4) {

    fixed[baseByte + 0] = original[baseByte + 3];
    fixed[baseByte + 1] = original[baseByte + 2];
    fixed[baseByte + 2] = original[baseByte + 1];
    fixed[baseByte + 3] = original[baseByte + 0];

  }
}
