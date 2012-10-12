/* 
   WK2-v0.2 -- Wilson-Kaplan-2 version 0.2

   Compresses buffers using a dictionary based match and partial match
   scheme.

   Scott F. Kaplan -- sfkaplan@cs.utexas.edu
   September 1997 */

/* ============================================================ */
/* Included files */

#include <unistd.h>
#include <math.h>
#include "WK2.h"

/* ============================================================ */
/* Types and structures */

/* A structure to store each element of the dictionary. */
typedef WK_word DictionaryElement;

/* ============================================================ */
/* Pre-processor constants */

#define BITS_PER_WORD 32
#define BYTES_PER_WORD 4
#define NUMBER_OF_SETS 4
#define ENTRIES_PER_SET 4
#define ALL_ONES_MASK 0xFFFFFFFF

#define ZERO_TAG 0x0
#define PARTIAL_TAG 0x1
#define MISS_TAG 0x2
#define EXACT_TAG 0x3

#define BITS_PER_BYTE 8

/* ============================================================ */
/* Global macros */

/* Strip the low bits of a pattern to give the high bits pattern.
   The stripped patterns are used for initial tests of partial
   matches. */
#define STRIP_LOW_BITS(pattern) (pattern & 0xFFFFFC00)

/* String the high bits of a pattern so the low order bits can
   be included in an encoding of a partial match. */
#define STRIP_HIGH_BITS(pattern) (pattern & 0x3FF)

/* This macro defined the contents of the initial index table.  The
   sets that compose the dictionary are stored in a single array.
   Given an index to some element in the dictionary, that index can be
   used with this table to obtain the index of the first element of
   that set in the dictionary array. */
#define INITIAL_INDEX_TABLE_CONTENTS { \
  0, 0, 0, 0, \
  4, 4, 4, 4, \
  8, 8, 8, 8, \
  12, 12, 12, 12 \
}

/* This macro defines the contents of the hash table, into which the
   hash function will look to determine in which set to search for a
   pattern.  The table contents has been generated through the
   pseudo-random number function lrand48(). */
#define HASH_TABLE_CONTENTS { \
  8, \
  0, \
  8, \
  4, \
  0, \
  8, \
  4, \
  8, \
  8, \
  0, \
  0, \
  12, \
  12, \
  0, \
  8, \
  8, \
  12, \
  4, \
  0, \
  4, \
  8, \
  4, \
  4, \
  4, \
  8, \
  4, \
  8, \
  0, \
  12, \
  4, \
  12, \
  8, \
  12, \
  0, \
  8, \
  0, \
  12, \
  4, \
  0, \
  0, \
  12, \
  0, \
  8, \
  4, \
  8, \
  8, \
  4, \
  8, \
  12, \
  12, \
  8, \
  4, \
  0, \
  8, \
  8, \
  8, \
  0, \
  0, \
  4, \
  12, \
  0, \
  12, \
  8, \
  8, \
  8, \
  8, \
  12, \
  4, \
  8, \
  8, \
  12, \
  12, \
  8, \
  12, \
  12, \
  4, \
  8, \
  4, \
  4, \
  8, \
  4, \
  8, \
  0, \
  0, \
  0, \
  0, \
  0, \
  4, \
  4, \
  4, \
  4, \
  4, \
  0, \
  12, \
  0, \
  12, \
  4, \
  12, \
  8, \
  0, \
  4, \
  4, \
  8, \
  4, \
  12, \
  4, \
  8, \
  12, \
  12, \
  8, \
  8, \
  0, \
  12, \
  4, \
  4, \
  8, \
  8, \
  8, \
  0, \
  4, \
  12, \
  8, \
  4, \
  0, \
  8, \
  0, \
  4, \
  0, \
  8, \
  4, \
  8, \
  12, \
  12, \
  12, \
  4, \
  0, \
  8, \
  12, \
  0, \
  0, \
  0, \
  12, \
  0, \
  4, \
  0, \
  12, \
  4, \
  0, \
  8, \
  4, \
  12, \
  0, \
  4, \
  0, \
  4, \
  12, \
  12, \
  8, \
  4, \
  8, \
  0, \
  4, \
  0, \
  0, \
  4, \
  0, \
  0, \
  0, \
  0, \
  12, \
  0, \
  12, \
  8, \
  0, \
  8, \
  4, \
  0, \
  8, \
  4, \
  4, \
  12, \
  0, \
  12, \
  0, \
  12, \
  0, \
  8, \
  12, \
  8, \
  8, \
  12, \
  4, \
  12, \
  12, \
  12, \
  8, \
  8, \
  12, \
  8, \
  12, \
  12, \
  12, \
  8, \
  0, \
  0, \
  4, \
  8, \
  0, \
  12, \
  4, \
  0, \
  4, \
  4, \
  0, \
  4, \
  12, \
  4, \
  0, \
  12, \
  12, \
  4, \
  0, \
  4, \
  0, \
  12, \
  0, \
  12, \
  8, \
  12, \
  0, \
  4, \
  12, \
  4, \
  4, \
  12, \
  12, \
  12, \
  0, \
  12, \
  4, \
  8, \
  0, \
  12, \
  4, \
  8, \
  0, \
  4, \
  4, \
  12, \
  8, \
  4, \
  12, \
  8, \
  12, \
  8, \
  0 \
}

/* Some output macros for debugging.  These macros are pre-processed
   into nothing if the DEBUG_PAGE_COMPRESS symbol is not defined. */
#if defined DEBUG_WK
#define DEBUG_PRINT_1(string) printf (string)
#define DEBUG_PRINT_2(string,value) printf(string, value)
#else
#define DEBUG_PRINT_1(string)
#define DEBUG_PRINT_2(string, value)
#endif

/* ============================================================ */
/* Macros for WK2_compress and WK2_decompress only */

/* Set up the dictionary before performing compression or
   decompression.  Each element is loaded with some value, the
   high-bits version of that value, and a next pointer. */
#define PRELOAD_DICTIONARY { \
  dictionary[0] = 1; \
  dictionary[1] = 1; \
  dictionary[2] = 1; \
  dictionary[3] = 1; \
  dictionary[4] = 1; \
  dictionary[5] = 1; \
  dictionary[6] = 1; \
  dictionary[7] = 1; \
  dictionary[8] = 1; \
  dictionary[9] = 1; \
  dictionary[10] = 1; \
  dictionary[11] = 1; \
  dictionary[12] = 1; \
  dictionary[13] = 1; \
  dictionary[14] = 1; \
  dictionary[15] = 1; \
}

/* In the set the starts at index initialIndex, copy the zeroth item
   into the first slot, leaving the zeroth slot open. */
#define SLIDE_TOP_ONE_DOWN \
  dictionary[initialIndex + 1] = \
    dictionary[initialIndex]

/* In the set that starts at index initialIndex, copy the first item
   into the second slot, and then the zeroth item into the first slot,
   leaving the zeroth slot open. */
#define SLIDE_TOP_TWO_DOWN { \
  dictionary[initialIndex + 2] = \
    dictionary[initialIndex + 1]; \
  SLIDE_TOP_ONE_DOWN; \
}

/* In the set that starts at index initialIndex, copy the second item
   into the third slot, the first item into the second slot, and then
   the zeroth item into the first slot, leaving the zeroth slot
   open. */
#define SLIDE_TOP_THREE_DOWN { \
  dictionary[initialIndex + 3] = \
    dictionary[initialIndex + 2]; \
  SLIDE_TOP_TWO_DOWN; \
}

/* ============================================================ */
/* WK2_compress macros and procedure */

/* When a word that missed completely is encountered, it needs to be
   inserted into the dictionary in favor of the LRU entry in that
   set.  Slide all other items in that set down, and insert this new
   pattern as the first entry in that set.
   Note that this macro is not the same one used to perform the same
   task when decompressing, as extra information (the high bits
   pattern and the set) are already available during compression, but
   will have to be calculated for decompression. */
#define INSERT_NEW_COMPRESSED_PATTERN { \
  SLIDE_TOP_THREE_DOWN; \
  dictionary[initialIndex] = sourcePattern; \
}

/* Given a word with its low bits set to zero, hash that word into an
   index (corresponding to the zero-th entry the appropriate set.  The
   hash into the table is determine by bits 10 through 21. */
#define HASH(pattern) \
  hashTable[((pattern) >> 10) & 0x000000FF]

/* Add the bits given by bitsToEncodeExpr to the destination buffer. */
#define ADD_TO_DESTINATION(bitsToEncodeExpr, numberOfBitsToEncode) { \
  if (numberOfBitsToEncode > remainingBits) { \
    /* The number of bits to be encoded exceeds the number of bits
       available in the current encoding word.  So, append as many bits
       as will fit into the current word, store that word, and then
       begin a new encoding word with the remaining bits. */ \
    unsigned int bitsInSecondWord = numberOfBitsToEncode - remainingBits; \
    WK_word bitsToEncode = (bitsToEncodeExpr); \
    *destinationBuffer = ((encodingWord << remainingBits) | \
			  (bitsToEncode >> bitsInSecondWord)); \
    DEBUG_PRINT_2("Crossing word boundary: %8x\n", *destinationBuffer); \
    destinationBuffer++; \
    /* It's safe not to mask out the high bits already stored in the
       previous encoding word, as those high bits will be shifted out
       later, as this encoding word is filled. */ \
    encodingWord = bitsToEncode; \
    remainingBits = BITS_PER_WORD - bitsInSecondWord; \
  } else if (numberOfBitsToEncode < remainingBits) { \
    /* The number of bits to be encoded is less than the number of
       bits remaining in the current encoding word.  Simply append
       all the bits to the current encoding word. */ \
    DEBUG_PRINT_1("Within word boundary\n"); \
    encodingWord <<= numberOfBitsToEncode; \
    encodingWord |= (bitsToEncodeExpr); \
    remainingBits -= numberOfBitsToEncode; \
  } else { \
    /* The number of bits to be encoded is exactly the number of
       bits remaining in the current encoding word.  Append the bits
       to the current encoding word, store that word, and initialize
       a new encoding word by resetting the remaining bits counter. */ \
    if (numberOfBitsToEncode == BITS_PER_WORD) { \
      *destinationBuffer = (bitsToEncodeExpr); \
    } else { \
      *destinationBuffer = ((encodingWord << remainingBits) | \
			    (bitsToEncodeExpr)); \
    } \
    DEBUG_PRINT_2("Hitting word boundary: %8x\n", *destinationBuffer); \
    destinationBuffer++; \
    remainingBits = BITS_PER_WORD; \
  } \
}


/* Create the encoding for a word that did not match any dictionary
   entry and add that information to the end of the desintation buffer. */
#define ENCODE_MISS { \
  DEBUG_PRINT_1("Encoding miss\n"); \
  ADD_TO_DESTINATION(0x2,2); \
  ADD_TO_DESTINATION(sourcePattern,BITS_PER_WORD); \
}

/* Create the encoding of a partial match and add that information
   to the end of the destination buffer. */
#define ENCODE_PARTIAL { \
  DEBUG_PRINT_2("Encoding partial: %1x\n", currentIndex); \
  ADD_TO_DESTINATION(0x4000 | \
		     (currentIndex << 10) | \
		     STRIP_HIGH_BITS(sourcePattern), \
		     16); \
}

/* Create the encoding of a partial match and add that information
   to the end of the destination buffer. */
#define ENCODE_EXACT { \
  DEBUG_PRINT_2("Encoding exact: %1x\n", currentIndex); \
  ADD_TO_DESTINATION(0x30 | currentIndex, 6); \
}

/* Create the encoding of an exact match at the first position and add
   that information to the end of the destination buffer. */
#define ENCODE_ZERO { \
  DEBUG_PRINT_1("Encoding zero\n"); \
  ADD_TO_DESTINATION(ZERO_TAG, 2); \
}

#define COPY_SOURCE_TO_ZEROTH_POSITION \
  dictionary[initialIndex] = sourcePattern


/* Attempt a partial match and, if needed, an exact match between
   the given pattern and the zeroth dictionary element in the given
   set. */
#define COMPARE_ZEROTH_ELEMENT { \
  currentIndex = initialIndex; \
  if (STRIP_LOW_BITS(dictionary[currentIndex]) ==  \
      sourceHighBitsPattern) { \
    if (dictionary[currentIndex] == sourcePattern) { \
      ENCODE_EXACT; \
    } else { \
      ENCODE_PARTIAL; \
      dictionary[currentIndex] = sourcePattern; \
    } \
    continue; \
  } \
}

/* Attempt a partial match and, if needed, an exact match between
   the given pattern and the first dictionary element in the given
   set.  Re-order the set if a match occurs. */
#define COMPARE_FIRST_ELEMENT { \
  currentIndex++; \
  if (STRIP_LOW_BITS(dictionary[currentIndex]) ==  \
      sourceHighBitsPattern) { \
    if (dictionary[currentIndex] == sourcePattern) { \
      ENCODE_EXACT; \
    } else { \
      ENCODE_PARTIAL; \
    } \
    SLIDE_TOP_ONE_DOWN; \
    COPY_SOURCE_TO_ZEROTH_POSITION; \
    continue; \
  } \
}

/* Attempt a partial match and, if needed, an exact match between
   the given pattern and the zeroth dictionary element in the given
   set.  Re-order the set if a match occurs. */
#define COMPARE_SECOND_ELEMENT { \
  currentIndex++; \
  if (STRIP_LOW_BITS(dictionary[currentIndex]) ==  \
      sourceHighBitsPattern) { \
    if (dictionary[currentIndex] == sourcePattern) { \
      ENCODE_EXACT; \
    } else { \
      ENCODE_PARTIAL; \
    } \
    SLIDE_TOP_TWO_DOWN; \
    COPY_SOURCE_TO_ZEROTH_POSITION; \
    continue; \
  } \
}

/* Attempt a partial match and, if needed, an exact match between
   the given pattern and the zeroth dictionary element in the given
   set.  Re-order the set if a match occurs. */
#define COMPARE_THIRD_ELEMENT { \
  currentIndex++; \
  if (STRIP_LOW_BITS(dictionary[currentIndex]) ==  \
      sourceHighBitsPattern) { \
    if (dictionary[currentIndex] == sourcePattern) { \
      ENCODE_EXACT; \
    } else { \
      ENCODE_PARTIAL; \
    } \
    SLIDE_TOP_THREE_DOWN; \
    COPY_SOURCE_TO_ZEROTH_POSITION; \
    continue; \
  } \
}

/* Given pointers to source buffer (sourceBuffer) of uncompressed data
   and a destination buffer (destinationBuffer) into which compressed
   data can be placed, as well as the number of words in the source
   buffer (words), compress the contents of the source and place the
   results in the destination.  Return the number of bytes placed into
   the destination. */
unsigned int
WK2_compress (WK_word* sourceBuffer,
	      WK_word* destinationBuffer,
	      unsigned int words) {
  DictionaryElement dictionary[DICTIONARY_SIZE];
  unsigned int hashTable [] = HASH_TABLE_CONTENTS;
  int wordIndex;
  unsigned int remainingBits = BITS_PER_WORD;
  WK_word encodingWord = 0;
  WK_word* destinationBufferStartingPoint = destinationBuffer;

  /* Initialize the elements with values and set its pointers. */
  PRELOAD_DICTIONARY;

  /* Loop through each word in the source page. */
  for (wordIndex = 0;  wordIndex < words; wordIndex++) {
    register WK_word sourcePattern;
    register WK_word sourceHighBitsPattern;
    unsigned int initialIndex;
    unsigned int currentIndex;

    /* Load the current pattern into a register. */
    sourcePattern = sourceBuffer[wordIndex];

    /* If this word is all zeros, encode it as such. */
    if (sourcePattern == 0) {
      ENCODE_ZERO;
      continue;
    }

    /* Load a partial matching pattern (i.e. the low bits have all
       been set to zero) into another register. */
    sourceHighBitsPattern = STRIP_LOW_BITS(sourcePattern);

    /* Determine which set to search in the dictionary through a hash
       function.  Note that the hash function returns the array index
       at which we begin searching, since the sets are stored linearly
       in an array. */
    initialIndex = HASH(sourcePattern);
    currentIndex = initialIndex;

    /* Compare the source word to each element in the set by comparing
       first the high bits and then the full pattern.  If a match is
       made, the encoding will be performed and a "continue" statement
       will cause a skip to the next word in the source page. */
    COMPARE_ZEROTH_ELEMENT; /* 0 */
    COMPARE_FIRST_ELEMENT; /* 1 */
    COMPARE_SECOND_ELEMENT; /* 2 */
    COMPARE_THIRD_ELEMENT; /* 3 */

    /* If we've reached this point, we've missed at every element
       of the dictionary.  Encode the miss and update the
       dictionary. */
    ENCODE_MISS;
    INSERT_NEW_COMPRESSED_PATTERN;

  }

  /* If there are any bits stored in the current encoding word, shift
     those bits to the top of the word and store them. */
  if (remainingBits < BITS_PER_WORD) {
    *destinationBuffer = (encodingWord << remainingBits);
    DEBUG_PRINT_2("Flushing last word: %8x\n", *destinationBuffer);
    destinationBuffer++;
  }

  /* Return the number of bytes placed into the compression buffer. */
  return ((unsigned int)(destinationBuffer -
			 destinationBufferStartingPoint)) *
    BYTES_PER_WORD;

}

/* ============================================================ */
/* WK2_decompress macros and procedure */

/* When a word that missed completely is encountered, it needs to be
   inserted into the dictionary in favor of the LRU entry in that
   set.  Slide all other items in that set down, and insert this new
   pattern as the first entry in that set.
   Note that this macro is not the same one used to perform the same
   task when compressing, as extra information (the high bits
   pattern and the set) are already available during compression, but
   will have to be calculated for decompression. */
#define INSERT_NEW_DECOMPRESSED_PATTERN { \
  initialIndex = HASH(*destinationBuffer); \
  SLIDE_TOP_THREE_DOWN; \
  dictionary[initialIndex] = *destinationBuffer; \
}

/* Based on some current index and the initial index for the set in
   which the current index resides, slide the elements of that set
   down such that the current index's contents are clobbered, and the
   zeroth slot in the set is open. */
#define SLIDE_SOME_NUMBER_OF_ELEMENTS \
  switch (currentIndex - initialIndex) { \
  case 3: \
    dictionary[initialIndex + 3] = \
      dictionary[initialIndex + 2]; \
  case 2: \
    dictionary[initialIndex + 2] = \
      dictionary[initialIndex + 1]; \
  case 1: \
    dictionary[initialIndex + 1] = \
      dictionary[initialIndex]; \
  } \

/* Extract the given number of bits from the source buffer and
   place those bits, by themselves, into the target variable. */
#define EXTRACT_BITS(numberOfBitsToExtract, targetVariable) { \
  if (numberOfBitsToExtract > remainingBits) { \
    /* The set of bits to be extracted are split across the current
       decoding word and the next one.  Extract the bits available in
       the current word and place them into their final position in
       the target variable.  Advance to the next decoding word in
       the source buffer and extract the remaining needed bits into
       the target variable. */ \
    unsigned int bitsInSecondWord = numberOfBitsToExtract - remainingBits; \
    targetVariable = (decodingWord >> \
		      (BITS_PER_WORD - numberOfBitsToExtract)); \
    sourceBuffer++; \
    decodingWord = *sourceBuffer; \
    DEBUG_PRINT_2("Crossing word boundary: %8x\n", decodingWord); \
    targetVariable |= (decodingWord >> (BITS_PER_WORD - bitsInSecondWord)); \
    decodingWord <<= bitsInSecondWord; \
    remainingBits = BITS_PER_WORD - bitsInSecondWord; \
  } else if (numberOfBitsToExtract < remainingBits) { \
    /* All of the bits to be extracted are available in the current
       decoding word.  Extract those bits, place them into the target,
       and update the current decoding word accordingly. */ \
    DEBUG_PRINT_1("Within word boundary\n"); \
    targetVariable = (decodingWord >> \
		      (BITS_PER_WORD - numberOfBitsToExtract)); \
    decodingWord <<= numberOfBitsToExtract; \
    remainingBits -= numberOfBitsToExtract; \
  } else { \
    /* The number of bits requested is exactly the number of bits left in
       the current decoding word.  Copy those bits and advance the
       current decoding word to the next word in the source buffer. */ \
    targetVariable = (decodingWord >> \
		      (BITS_PER_WORD - numberOfBitsToExtract)); \
    sourceBuffer++; \
    decodingWord = *sourceBuffer; \
    DEBUG_PRINT_2("Hitting word boundary: %8x\n", decodingWord); \
    remainingBits = BITS_PER_WORD; \
  } \
}

/* Given a pointer to a source buffer (sourceBuffer) of compressed
   data and a pointer to a destination buffer (destinationBuffer) into
   which uncompressed data can be placed, as well as the number of
   uncompressed words that will be written to the destination buffer
   (words), decompress the data into the destination buffer. */
void
WK2_decompress (WK_word* sourceBuffer,
		WK_word* destinationBuffer,
		unsigned int words) {
  /* The dictionary array is divided into sets.  Each entry in the
     dictionary array is really an entry in one of the dictionary's
     sets.  Each set begins at a particular offset within the
     dictionary array.  Given an index into the dictionary array (and
     thus into a set), the initial index table provides the index at
     which that set begins in the dictionary array. */
  DictionaryElement dictionary[DICTIONARY_SIZE];
  unsigned int initialIndexTable [] = INITIAL_INDEX_TABLE_CONTENTS;
  unsigned int hashTable [] = HASH_TABLE_CONTENTS;
  unsigned int wordIndex;
  unsigned int remainingBits = BITS_PER_WORD;
  WK_word decodingWord = *sourceBuffer;
  
  DEBUG_PRINT_2("\n-----\nBeginning decompression: %8x\n", decodingWord);

  /* Initialize the elements with values and set its pointers. */
  PRELOAD_DICTIONARY;

  /* Loop through each encoded word in the source buffer, and iterate
     through the words of the destination buffer as decompression
     occurs. */
  for (wordIndex = 0;
       wordIndex < words;
       wordIndex++, destinationBuffer++) {
    unsigned int tag;
    unsigned int currentIndex;
    unsigned int initialIndex;
    register WK_word lowBitsWord;
    register WK_word highBitsWord;

    /* Extract the tag from the current word. */
    EXTRACT_BITS(2, tag);

    /* Based on that tag, determine how to decode the rest of the
       word. */
    switch (tag) {

    case ZERO_TAG:
      /* Append a zero word to the destination. */
      *destinationBuffer = 0;
      break;

    case MISS_TAG:
      DEBUG_PRINT_1("Decoding miss\n");
      /* Extract the whole word and append it to the destination
	 page. */
      EXTRACT_BITS(BITS_PER_WORD, *destinationBuffer);
      /* Update the dictionary by inserting this pattern in place of
	 the oldest pattern in the LRU queue, and making that position
	 the new head. */
      INSERT_NEW_DECOMPRESSED_PATTERN;
      break;

    case PARTIAL_TAG:
      /* Extract the index of the dictionary entry that this word
	 matched. */
      EXTRACT_BITS(4, currentIndex);
      DEBUG_PRINT_2("Decoding partial: %1x\n", currentIndex);
      /* Extract the low bits. */
      EXTRACT_BITS(10, lowBitsWord);
      /* Grab the high bits pattern from the proper dictionary
	 entry. */
      highBitsWord = 
	STRIP_LOW_BITS(dictionary[currentIndex]);
      /* Append to the destination page the combination of the high
	 and low bits words. */
      *destinationBuffer = highBitsWord | lowBitsWord;
      /* Update the dictionary by moving the accessed entry to the
	 head of its set.  Note that we also update the full pattern
	 with the word that was just reconstructed. */
      initialIndex = initialIndexTable[currentIndex];
      SLIDE_SOME_NUMBER_OF_ELEMENTS;
      dictionary[initialIndex] = *destinationBuffer;
      break;

    case EXACT_TAG:
      /* Extract the index of the dictionary entry that this word
	 matched. */
      EXTRACT_BITS(4, currentIndex);
      DEBUG_PRINT_2("Decoding exact: %1x\n", currentIndex);
      /* Grab the pattern from the given entry in the dictionary
	 and append it to the destination page. */
      *destinationBuffer = dictionary[currentIndex];
      /* Update the dictionary by moving the accessed entry to the
	 head of its set. */
      initialIndex = initialIndexTable[currentIndex];
      SLIDE_SOME_NUMBER_OF_ELEMENTS;
      dictionary[initialIndex] = *destinationBuffer;
      break;

    default:
      DEBUG_PRINT_2("***Bad tag value: %1x\n", tag);
      exit (-1);

    }
  }
}

/* ============================================================ */
/* WK2_measureCompress macros and procedure */

/* Add to the exact or partial count for this queue position. */
#define COUNT_EXACT { \
  exactMatchCountArray[currentIndex]++; \
  bitsUsedForEncoding += 6; \
}

#define COUNT_ZERO bitsUsedForEncoding += 2

#define COUNT_PARTIAL { \
  partialMatchCountArray[currentIndex]++; \
  bitsUsedForEncoding += 16; \
}

#define COUNT_MISS bitsUsedForEncoding += 34

/* Attempt a partial match and, if needed, an exact match between
   the given pattern and the zeroth dictionary element in the given
   set. */
#define COMPARE_ZEROTH_ELEMENT_FOR_MEASUREMENT { \
  currentIndex = initialIndex; \
  if (STRIP_LOW_BITS(dictionary[currentIndex]) ==  \
      sourceHighBitsPattern) { \
    if (dictionary[currentIndex] == sourcePattern) { \
      COUNT_EXACT; \
    } else { \
      COUNT_PARTIAL; \
      dictionary[currentIndex] = sourcePattern; \
    } \
    continue; \
  } \
}

/* Attempt a partial match and, if needed, an exact match between
   the given pattern and the first dictionary element in the given
   set.  Re-order the set if a match occurs. */
#define COMPARE_FIRST_ELEMENT_FOR_MEASUREMENT { \
  currentIndex++; \
  if (STRIP_LOW_BITS(dictionary[currentIndex]) ==  \
      sourceHighBitsPattern) { \
    if (dictionary[currentIndex] == sourcePattern) { \
      COUNT_EXACT; \
    } else { \
      COUNT_PARTIAL; \
    } \
    SLIDE_TOP_ONE_DOWN; \
    COPY_SOURCE_TO_ZEROTH_POSITION; \
    continue; \
  } \
}

/* Attempt a partial match and, if needed, an exact match between
   the given pattern and the zeroth dictionary element in the given
   set.  Re-order the set if a match occurs. */
#define COMPARE_SECOND_ELEMENT_FOR_MEASUREMENT { \
  currentIndex++; \
  if (STRIP_LOW_BITS(dictionary[currentIndex]) ==  \
      sourceHighBitsPattern) { \
    if (dictionary[currentIndex] == sourcePattern) { \
      COUNT_EXACT; \
    } else { \
      COUNT_PARTIAL; \
    } \
    SLIDE_TOP_TWO_DOWN; \
    COPY_SOURCE_TO_ZEROTH_POSITION; \
    continue; \
  } \
}

/* Attempt a partial match and, if needed, an exact match between
   the given pattern and the zeroth dictionary element in the given
   set.  Re-order the set if a match occurs. */
#define COMPARE_THIRD_ELEMENT_FOR_MEASUREMENT { \
  currentIndex++; \
  if (STRIP_LOW_BITS(dictionary[currentIndex]) ==  \
      sourceHighBitsPattern) { \
    if (dictionary[currentIndex] == sourcePattern) { \
      COUNT_EXACT; \
    } else { \
      COUNT_PARTIAL; \
    } \
    SLIDE_TOP_THREE_DOWN; \
    COPY_SOURCE_TO_ZEROTH_POSITION; \
    continue; \
  } \
}

/* Given a pointer to a source buffer (sourceBuffer) of uncompressed
   data, the number of words stored in that buffer (words), and two
   arrays for counting the number of exact (exactMatchCountArray) and
   partial (partialMatchCountArray) matches at each LRU position,
   compress the source and record what types of hits (partial or
   exact) occurred at each queue position.  Return the number of words
   that would be placed into a destination buffer (if there were
   one). */
unsigned int
WK2_measureCompress (WK_word* sourceBuffer,
		     unsigned int words,
		     unsigned int* exactMatchCountArray,
		     unsigned int* partialMatchCountArray) {
  DictionaryElement dictionary[DICTIONARY_SIZE];
  unsigned int hashTable [] = HASH_TABLE_CONTENTS;
  unsigned int wordIndex;
  unsigned int index;
  unsigned int bitsUsedForEncoding = 0;

  /* Initialize the elements with values and set its pointers. */
  PRELOAD_DICTIONARY;

  /* Loop through the match information arrays and clear their
     values. */
  for (index = 0; index < DICTIONARY_SIZE; index++) {
    exactMatchCountArray[index] = 0;
    partialMatchCountArray[index] = 0;
  }

  /* Loop through each word in the source page. */
  for (wordIndex = 0;  wordIndex < words; wordIndex++) {
    register WK_word sourcePattern;
    register WK_word sourceHighBitsPattern;
    unsigned int initialIndex;
    unsigned int currentIndex;

    /* Load the current pattern into a register. */
    sourcePattern = sourceBuffer[wordIndex];

    /* If this word is all zeros, encode it as such. */
    if (sourcePattern == 0) {
      COUNT_ZERO;
      continue;
    }

    /* Load a partial matching pattern (i.e. the low bits have all
       been set to zero) into another register. */
    sourceHighBitsPattern = STRIP_LOW_BITS(sourcePattern);

    /* Determine which set to search in the dictionary through a hash
       function.  Note that the hash function returns the array index
       at which we begin searching, since the sets are stored linearly
       in an array. */
    initialIndex = HASH(sourcePattern);
    currentIndex = initialIndex;

    /* Compare the source word to each element in the set by comparing
       first the high bits and then the full pattern.  If a match is
       made, the encoding will be performed and a "continue" statement
       will cause a skip to the next word in the source page. */
    COMPARE_ZEROTH_ELEMENT_FOR_MEASUREMENT; /* 0 */
    COMPARE_FIRST_ELEMENT_FOR_MEASUREMENT; /* 1 */
    COMPARE_SECOND_ELEMENT_FOR_MEASUREMENT; /* 2 */
    COMPARE_THIRD_ELEMENT_FOR_MEASUREMENT; /* 3 */

    /* If we've reached this point, we've missed at every element
       of the dictionary.  Encode the miss and update the
       dictionary. */
    COUNT_MISS;
    INSERT_NEW_COMPRESSED_PATTERN;

  }

  /* Return the number of bytes used to store the compressed
     region. */
  return ((unsigned int)ceil((double)bitsUsedForEncoding /
                             (double)BITS_PER_WORD)) *
    BYTES_PER_WORD;

}
