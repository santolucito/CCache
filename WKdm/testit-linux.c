
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "WKdm.h"
#include "timer.h"


void timeCompression (WK_word* source,
		      WK_word* destination,
		      unsigned int words,
		      unsigned int iterations) {
  zenTimerType myTimer;
  unsigned int iteration;
  unsigned int compressedSize;
  unsigned long long max, min, total, mean, result;
  
  max = min = total = 0;

  for (iteration = 0; iteration < iterations; iteration++) {
    
    START_ZEN_TIME(myTimer);
    compressedSize = WKdm_compress (source, destination, words);
    STOP_ZEN_TIME(myTimer,result);
    if (result > max)
      max = result;
    if ((result < min) || (min == 0))
      min = result;
    total += result;

  }

  mean = total / iterations;
  printf("Timer was called %ld times\n", iterations);
  printf("\n");
  printf("For an average of %ld cycles per call\n", mean);
  printf("The minimum time was: %ld\n", min);
  printf("The maximum time was: %ld\n", max);
  printf("The compressed size was: %ld\n", compressedSize);
  printf("\n");  

}


void timeDecompression (WK_word* source,
			WK_word* destination,
			unsigned int words,
			unsigned int iterations) {
  zenTimerType myTimer;
  size_t pageSize = getpagesize();
  unsigned int iteration;
  unsigned long long max, min, total, mean, result;
  
  max = min = total = 0;

  for (iteration = 0; iteration < iterations; iteration++) {
    
    START_ZEN_TIME(myTimer);
    WKdm_decompress (source, destination, words);
    STOP_ZEN_TIME(myTimer,result);
    if (result > max)
      max = result;
    if ((result < min) || (min == 0))
      min = result;
    total += result;

  }

  mean = total / iterations;
  printf("Timer was called %ld times\n", iterations);
  printf("\n");
  printf("For an average of %ld cycles per call\n", mean);
  printf("The minimum time was: %ld\n", min);
  printf("The maximum time was: %ld\n", max);
  printf("\n");

}


void compare (WK_word* source,
	      WK_word* destination,
	      unsigned int words) {

  unsigned int compressSize;

  /* Make a copy of the source page so that we have a version
     of the source that is guaranteed to be unmolested. */
  WK_word* copy = (WK_word*)malloc(sizeof (WK_word) * words);
  memcpy ((void*)copy, (void*)source, sizeof (WK_word) * words);

  /* Compress and then decompress the data. */
  compressSize = WKdm_compress (copy, destination, words);
  WKdm_decompress (destination, copy, words);

  /* Compare the decompressed data to the original. */
  if (memcmp ((void*)source, (void*)copy, sizeof (WK_word) * words) == 0) {
    printf ("Matched\n");
  } else {
    unsigned int word;
    printf ("Differed...sadness...\n");
    printf ("\tsource\t\tcopy\n");
    for (word = 0; word < words; word++) {
      printf("\t%8lx\t%8lx\n", source[word], copy[word]);
    }
  }

  printf("\n");

}


void testit (WK_word* source,
	     WK_word* destination,
	     unsigned int words,
	     unsigned int iterations) {

  compare (source, destination, words);
  timeCompression (source, destination, words, iterations);
  timeDecompression (destination, source, words, iterations);

}


void main () {
  WK_word* a;
  WK_word* b;
  unsigned int word;
  unsigned int words;
  unsigned int iterations = 1;
  unsigned int i;

  printf("How many words? ");
  scanf("%d",&words);

  a = (WK_word*) malloc (sizeof(WK_word) *
				  words);
  b = (WK_word*) malloc (sizeof(WK_word) *
				  words * 2);

  for (i = 0; i < words; i++) {
    a[i] = 0;
  }
  printf("All zeros:\n\n");
  testit (a, b, words, iterations);

  for (i = 0; i < words; i++) {
    a[i] = (WK_word)lrand48();
  }
  printf("Random data:\n\n");
  testit (a, b, words, iterations);

  for (i = 0; i < words; i++) {
    switch (lrand48() % 4) {
    case 0:
      a[i] = 0xf5ba0132;
      break;
    case 1:
      a[i] = 0x10832;
      break;
    case 2:
      a[i] = 0xf5ba03e7;
      break;
    case 3:
      a[i] = 0x55555555;
      break;
    }
  }
  printf("Somewhat patterned:\n\n");
  testit (a, b, words, iterations);

}
