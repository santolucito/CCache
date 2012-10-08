CC = g++
CFLAGS = -g -O2
INCLUDEFLAGS = -I ./lzo/include/ -I ./WKdm/ -I ./WK2/ -I ./lzrw1/
LDFLAGS = -llzo -lz -lm -lrt
LDPATH = -L ./lzo/
OBJS = test-compression.o TraceRecord.o CompressionAlgorithmTester.o safezlib.o \
	WKdmTester.o LZOTester.o LZRW1Tester.o WK2Tester.o
COMPRESSORS = WKdm/WKdm.o \
		WK2/WK2.o \
		lzrw1/lzrw1.o 
EXECUTABLE = ./test-compression
#EXECUTABLE = ../../$(HOSTTYPE)/test-compression

default: $(OBJS)
	$(CC) $(LDPATH) $(CFLAGS) -o $(EXECUTABLE) $(OBJS) $(COMPRESSORS) $(LDFLAGS)

clean:
	rm -f *.o *~ $(EXECUTABLE)

.cpp.o:
	$(CXX) $(COMPILERFLAGS) $(INCLUDES) $<

test-compression.o: test-compression.cc \
		    TraceRecord.hh \
		    CompressionAlgorithmTester.hh
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) -c test-compression.cc

TraceRecord.o: TraceRecord.cc TraceRecord.hh
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) -c TraceRecord.cc

CompressionAlgorithmTester.o: CompressionAlgorithmTester.cc \
			      CompressionAlgorithmTester.hh 
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) -c CompressionAlgorithmTester.cc

safezlib.o: safezlib.cc safezlib.hh
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) -c safezlib.cc

WKdmTester.o: CompressionAlgorithmTester.hh \
	      CompressionAlgorithmTester.cc \
	      WKdmTester.hh \
	      WKdmTester.cc
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) -c WKdmTester.cc

WKdm-24-8Tester.o: CompressionAlgorithmTester.hh \
	      CompressionAlgorithmTester.cc \
	      WKdm-24-8Tester.hh \
	      WKdm-24-8Tester.cc
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) -c WKdm-24-8Tester.cc

LZOTester.o: CompressionAlgorithmTester.hh \
	     CompressionAlgorithmTester.cc \
	     LZOTester.hh \
	     LZOTester.cc
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) -c LZOTester.cc

LZRW1Tester.o: CompressionAlgorithmTester.hh \
	       CompressionAlgorithmTester.cc \
	       LZRW1Tester.hh \
	       LZRW1Tester.cc
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) -c LZRW1Tester.cc

WK2Tester.o: CompressionAlgorithmTester.hh \
	     CompressionAlgorithmTester.cc \
	     LZRW1Tester.hh \
	     LZRW1Tester.cc
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) -c WK2Tester.cc

