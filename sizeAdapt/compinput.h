#ifndef COMPINPUT_H
#define COMPINPUT_H

#include "general.h"
#include <iostream.h>
#include <stdio.h>

struct EventInfo {
  bool compulsory;
  BlockNumber fetched_page_number;
  unsigned int fetched_compressed_size;
  double decompression_time;
  char eviction_tag;
  BlockNumber evicted_page_number;
  unsigned int evicted_compressed_size;
  double compression_time;
};


// "CompInput" hides the details of reading the compression trace from input. 
// If we end up having more than one methods of input they could all be 
// expressed as subclasses of the CompInput class
class CompInput {
  istream* in_stream;
public:
  CompInput (istream* input_stream) {
    in_stream = input_stream;
  }

  bool end_of_input() { return !in_stream->good(); }
  EventInfo get_next() { 
    EventInfo temp;
    char buffer[256];
    char compulsory_tag;  
    in_stream->getline(buffer, 256);
    if (buffer[0] == '(') // First line of input
      in_stream->getline(buffer, 256);
    if (buffer[0] == ')') {
      in_stream->getline(buffer, 256);
      // ASSERT(end_of_input());
      return temp;
    }
    sscanf(buffer, "  ( %c %x %d %lf %c %x %d %lf )\n",
	   &compulsory_tag,
	   &(temp.fetched_page_number),
	   &(temp.fetched_compressed_size),
	   &(temp.decompression_time),
	   &(temp.eviction_tag),
	   &(temp.evicted_page_number),
	   &(temp.evicted_compressed_size),
	   &(temp.compression_time));
    // ASSERT (compulsory_tag == 'C' || compulsory_tag == 'F');
    temp.compulsory = (compulsory_tag == 'C');
    return temp;
  }
};

#endif

/* Test code
   int main() {
   CompInput my_in(&cin);
   do {
   EventInfo event = my_in.get_next();
   if (my_in.end_of_input())
   break;
   cout << event.fetched_page_number << " " << event.compulsory << " " << 
   event.compression_time << endl;
   } while (1);
   }
*/

