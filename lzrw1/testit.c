#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lzrw1.h"



void compare (UBYTE* source,
	      UBYTE* destination,
	      ULONG bytes) {

  ULONG compressedSize;
  ULONG decompressedSize;

  /* Make a copy of the source page so that we have a version
     of the source that is guaranteed to be unmolested. */
  UBYTE* copy = (UBYTE*)malloc(sizeof(UBYTE) * bytes);
  memcpy ((void*)copy, (void*)source, sizeof (UBYTE) * bytes);

  /* Compress and then decompress the data. */
  lzrw1_compress(copy, bytes, destination, &compressedSize);
  lzrw1_decompress(destination,
		   compressedSize,
		   copy,
		   &decompressedSize);

  /* Compare the decompressed data to the original. */
  if (bytes != decompressedSize) {
    printf("Differed in size!\n");
  }
  if (memcmp ((void*)source, (void*)copy, sizeof (UBYTE) * bytes) == 0) {
    printf ("Matched\n");
  } else {
    unsigned int byte;
    printf ("Differed...sadness...\n");
    printf ("\tsource\t\tcopy\n");
    for (byte = 0; byte < bytes; byte++) {
      printf("\t%8lx\t%8lx\n", source[byte], copy[byte]);
    }
  }

  printf("\n");

}


void testit (UBYTE* source,
	     UBYTE* destination,
	     unsigned int bytes,
	     unsigned int iterations) {

  compare (source, destination, bytes);

}


void main () {
  UBYTE* a;
  UBYTE* b;
  unsigned int bytes;
  unsigned int iterations = 1;
  unsigned int i;

  printf("How many bytes? ");
  scanf("%d",&bytes);

  /*
  printf("How many iterations? ");
  scanf("%d", &iterations);
  */

  a = (UBYTE*)malloc(sizeof(UBYTE) * bytes);
  b = (UBYTE*)malloc(sizeof(UBYTE) * bytes * 2);

  for (i = 0; i < bytes; i++) {
    a[i] = 0;
  }
  printf("All zeros:\n\n");
  testit (a, b, bytes, iterations);

  for (i = 0; i < bytes; i++) {
    switch (lrand48() % 4) {
    case 0:
      a[i] = 0x32;
      break;
    case 1:
      a[i] = 0x33;
      break;
    case 2:
      a[i] = 0xe7;
      break;
    case 3:
      a[i] = 0x55;
      break;
    }
  }
  printf("Somewhat patterned:\n\n");
  testit (a, b, bytes, iterations);

}
