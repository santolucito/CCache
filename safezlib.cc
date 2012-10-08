// safezlib.cc
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu

// Provide an interface to the zlib library that performs error
// checking with each call.  The assumption is that an error is cause
// to halt the program, and so calls to these functions will rid the
// caller of the need to write error checking conditionals, thus
// cluttering the code.

using namespace std;

#include "safezlib.hh"
#include <cstdio>
#include <stdlib.h> 


gzFile
safegzopen(char* pathname, char* mode) {

  gzFile file = gzopen(pathname, mode);
  if (file == NULL) {
    cerr << "safegzopen: error " << pathname << endl;
    exit(-1);
  }

  return file;

}



void
safegzgets(gzFile file,
	   char* buffer,
	   unsigned int length) {

  char* result = gzgets(file, buffer, length);
  if (result == Z_NULL) {
    cerr << "safegzgets: error" << endl;
    exit(-1);
  }
}



void
safegzputs(gzFile file,
	   char* buffer) {

  int result = gzputs(file, buffer);
  if (result == -1) {
    cerr << "safegzputs: error" << endl;
    exit(-1);
  }
}



void
safegzread (gzFile file, voidp buffer, unsigned length) {

  int result = gzread(file, buffer, length);
  if (result != length) {
    cerr << "safegzread: error" << endl;
    exit(-1);
  }
}



void
safegzwrite (gzFile file, const voidp buffer, unsigned length) {

  int result = gzwrite(file, buffer, length);
  if (result != length) {
    cerr << "safegzwrite: error" << endl;
    exit(-1);
  }
}



int
safegzgetc (gzFile file) {

  int result = gzgetc(file);
  if (result == -1) {
    cerr << "safegzgetc: error" << endl;
    exit(-1);
  }

  return result;

}



void
safegzputc (gzFile file, int character) {

  int result = gzputc(file, character);
  if (result != character) {
    cerr << "safegzputc: error" << endl;
    exit(-1);
  }
}
