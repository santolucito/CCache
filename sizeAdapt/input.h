#ifndef INPUT_H
#define INPUT_H

#include "general.h"
#include <iostream.h>

// "Input" hides the details of reading the block numbers from input. If we end
// up having more than one methods of input they could all be expressed as
// subclasses of the Input class
class Input {
  istream* in_stream;
public:
  Input (istream* input_stream) {
    in_stream = input_stream;
  }

  bool end_of_input() { return !in_stream->good(); }
  BlockNumber get_next() { 
    BlockNumber num; 
    *in_stream >> hex >> num;
    return num;
  }
};

#endif

