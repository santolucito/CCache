// safezlib.hh
// Scott F. Kaplan -- sfkaplan@cs.utexas.edu

// Provide an interface to the zlib library that performs error
// checking with each call.  The assumption is that an error is cause
// to halt the program, and so calls to these functions will rid the
// caller of the need to write error checking conditionals, thus
// cluttering the code.



#include <iostream>
#include "zlib.h"



gzFile safegzopen (char* pathname, char* mode);
void safegzgets (gzFile file, char* buffer, unsigned length);
void safegzputs (gzFile file, char* buffer);
void safegzread (gzFile file, voidp buffer, unsigned length);
void safegzwrite (gzFile file, const voidp buffer, unsigned length);
int safegzgetc (gzFile file);
void safegzputc (gzFile file, int character);
