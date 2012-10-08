// A simple class that reads from a FILE stream into a set of fields
// that can be accessed publicly.  The intent is to hide the details
// of reading the trace format, which is why all of the data members
// that store trace information are made public.

#if !defined (_TRACERECORD_HH)
#define _TRACERECORD_HH

#include <stdio.h>
#include "zlib.h"

class TraceRecord {

public:

  TraceRecord(unsigned int initTracePageSize);
  ~TraceRecord();

  void read(gzFile inputFile, char tag);

  bool compulsory;
  char fetchedPageNumber[5];
  void* fetchedImage;
  void* fixedFetchedImage;
  bool evicted;
  bool dirty;
  char evictedPageNumber[5];
  void* evictedImage;
  void* fixedEvictedImage;

private:

  void readPageImage(gzFile inputFile,
		     void* returnImage,
		     void* fixedReturnImage);

  const unsigned int tracePageSize;

};

#endif // _TRACERECORD_HH
