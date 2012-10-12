#include <iostream.h>
#include <stdlib.h>

const unsigned int tableSize = 256;
const unsigned int setSize = 4;
const unsigned int numberOfSets = 4;

void main () {

  // Initialize the random number generator.
  srand48(0);

  // Loop enough times to create enough random numbers to fill the
  // table.
  for (unsigned int i = 0; i < tableSize; i++) {

    // Output a table entry.  Note that each entry will be used as an
    // index to the beginning of some set in a contiguous array, thus
    // the arithmetic that transforms each random value into one of
    // numberOfSets values.
    unsigned int indexValue = (lrand48() % numberOfSets) * setSize;
    cout << "  " << indexValue;

    // If this isn't the last item, output a comma separator.
    if (i < tableSize - 1) {
      cout << ',';
    }

    // Output a newline in C/C++-macro format.
    cout << " \\" << endl;

  }

}
