/* direct-mapped partial matching compressor with simple 22/10 split
 *
 *  Compresses buffers using a dictionary based match and partial match
 *  (high bits only or full match) scheme.
 *
 *  Paul Wilson -- wilson@cs.utexas.edu
 *  Scott F. Kaplan -- sfkaplan@cs.utexas.edu
 *  September 1997
 */

/* compressed output format, in memory order
 *  1. a four-word HEADER containing four one-word values:
 *     i.   a one-word code saying what algorithm compressed the data
 *     ii.  an integer WORD offset into the page saying
 *          where the queue position area starts
 *     iii. an integer WORD offset into the page saying where
 *          the low-bits area starts
 *     iv.  an integer WORD offset into the page saying where the
 *          low-bits area ends
 *
 *  2. a 64-word TAGS AREA holding one two-bit tag for each word in 
 *     the original (1024-word) page, packed 16 per word
 *
 *  3. a variable-sized FULL WORDS AREA (always word aligned and an
 *     integral number of words) holding full-word patterns that
 *     were not in the dictionary when encoded (i.e., dictionary misses)
 *
 *  4. a variable-sized QUEUE POSITIONS AREA (always word aligned and
 *     an integral number of words) holding four-bit queue positions,
 *     packed eight per word.
 *
 *  5. a variable-sized LOW BITS AREA (always word aligned and an
 *     integral number of words) holding ten-bit low-bit patterns
 *     (from partial matches), packed three per word. 
 */



/* ============================================================ */
/* Included files */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <strings.h>

typedef unsigned long WK_word;

/* at the moment we have dependencies on the page size.  That should
 * be changed to work for any power-of-two size that's at least 16
 * words, or something like that
 */

#define PAGE_SIZE_IN_WORDS 1024
#define PAGE_SIZE_IN_BYTES 4096

#define DICTIONARY_SIZE 16

/*
 * macros defining the basic layout of stuff in a page
 */
#define HEADER_SIZE_IN_WORDS 4
#define TAGS_AREA_OFFSET 4
#define TAGS_AREA_SIZE 64

/* the next few are used during compression to write the header */
#define SET_QPOS_AREA_START(compr_dest_buf,qpos_start_addr)  \
        (compr_dest_buf[1] = qpos_start_addr - compr_dest_buf)
#define SET_LOW_BITS_AREA_START(compr_dest_buf,lb_start_addr) \
        (compr_dest_buf[2] = lb_start_addr - compr_dest_buf)
#define SET_LOW_BITS_AREA_END(compr_dest_buf,lb_end_addr) \
        (compr_dest_buf[3] = lb_end_addr - compr_dest_buf)

/* the next few are only use during decompression to read the header */
#define TAGS_AREA_START(decomp_src_buf)       \
        (decomp_src_buf + TAGS_AREA_OFFSET)
#define TAGS_AREA_END(decomp_src_buf)         \
        (TAGS_AREA_START(decomp_src_buf) + TAGS_AREA_SIZE)
#define FULL_WORD_AREA_START(the_buf) TAGS_AREA_END(the_buf)
#define QPOS_AREA_START(decomp_src_buf)       \
        (decomp_src_buf + decomp_src_buf[1])   
#define LOW_BITS_AREA_START(decomp_src_buf)   \
        (decomp_src_buf + (decomp_src_buf[2]))
#define QPOS_AREA_END(the_buf) LOW_BITS_AREA_START(the_buf)
#define LOW_BITS_AREA_END(decomp_src_buf)     \
        (decomp_src_buf + (decomp_src_buf[3]))

/* ============================================================ */
/* Types and structures */

/* A structure to store each element of the dictionary. */
typedef WK_word DictionaryElement;

/* ============================================================ */
/* Misc constants */

#define BITS_PER_WORD 32
#define BYTES_PER_WORD 4
#define NUM_LOW_BITS 10
#define LOW_BITS_MASK 0x3FF
#define ALL_ONES_MASK 0xFFFFFFFF

#define TWO_BITS_PACKING_MASK 0x03030303
#define FOUR_BITS_PACKING_MASK 0x0F0F0F0F
#define TEN_LOW_BITS_MASK 0x000003FF
#define TWENTY_TWO_HIGH_BITS_MASK 0xFFFFFC00

/* Tag values.  NOTE THAT CODE MAY DEPEND ON THE NUMBERS USED.
 * Check for conditionals doing arithmetic on these things
 * before changing them
 */
#define ZERO_TAG 0x0
#define PARTIAL_TAG 0x1
#define MISS_TAG 0x2
#define EXACT_TAG 0x3

#define BITS_PER_BYTE 8

/* ============================================================ */
/* Global macros */

/* Shift out the low bits of a pattern to give the high bits pattern.
   The stripped patterns are used for initial tests of partial
   matches. */
#define HIGH_BITS(word_pattern) (word_pattern >> NUM_LOW_BITS)

/* String the high bits of a pattern so the low order bits can
   be included in an encoding of a partial match. */
#define LOW_BITS(word_pattern) (word_pattern & LOW_BITS_MASK)

#if defined DEBUG_WK
#define DEBUG_PRINT_1(string) printf (string)
#define DEBUG_PRINT_2(string,value) printf(string, value)
#else
#define DEBUG_PRINT_1(string)
#define DEBUG_PRINT_2(string, value)
#endif

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

/* these are the constants for the hash function lookup table.
 * Only zero maps to zero.  The rest of the tabale is the result
 * of appending 17 randomizations of the multiples of 4 from
 * 4 to 56.  Generated by a Scheme script in hash.scm. 
 */
#define HASH_LOOKUP_TABLE_CONTENTS { \
   0, 52,  8, 56, 16, 12, 28, 20,  4, 36, 48, 24, 44, 40, 32, 60, \
   8, 12, 28, 20,  4, 60, 16, 36, 24, 48, 44, 32, 52, 56, 40, 12, \
   8, 48, 16, 52, 60, 28, 56, 32, 20, 24, 36, 40, 44,  4,  8, 40, \
  60, 32, 20, 44,  4, 36, 52, 24, 16, 56, 48, 12, 28, 16,  8, 40, \
  36, 28, 32, 12,  4, 44, 52, 20, 24, 48, 60, 56, 40, 48,  8, 32, \
  28, 36,  4, 44, 20, 56, 60, 24, 52, 16, 12, 12,  4, 48, 20,  8, \
  52, 16, 60, 24, 36, 44, 28, 56, 40, 32, 36, 20, 24, 60, 40, 44, \
  52, 16, 32,  4, 48,  8, 28, 56, 12, 28, 32, 40, 52, 36, 16, 20, \
  48,  8,  4, 60, 24, 56, 44, 12,  8, 36, 24, 28, 16, 60, 20, 56, \
  32, 40, 48, 12,  4, 44, 52, 44, 40, 12, 56,  8, 36, 24, 60, 28, \
  48,  4, 32, 20, 16, 52, 60, 12, 24, 36,  8,  4, 16, 56, 48, 44, \
  40, 52, 32, 20, 28, 32, 12, 36, 28, 24, 56, 40, 16, 52, 44,  4, \
  20, 60,  8, 48, 48, 52, 12, 20, 32, 44, 36, 28,  4, 40, 24,  8, \
  56, 60, 16, 36, 32,  8, 40,  4, 52, 24, 44, 20, 12, 28, 48, 56, \
  16, 60,  4, 52, 60, 48, 20, 16, 56, 44, 24,  8, 40, 12, 32, 28, \
  36, 24, 32, 12,  4, 20, 16, 60, 36, 28,  8, 52, 40, 48, 44, 56  \
}

#define HASH_TO_DICT_BYTE_OFFSET(pattern) \
        (hashLookupTable[((pattern) >> 10) & 0xFF])

/* EMIT... macros emit bytes or words into the intermediate arrays
 */

#define EMIT_BYTE(fill_ptr, byte_value) {*fill_ptr = byte_value; fill_ptr++;}
#define EMIT_WORD(fill_ptr,word_value) {*fill_ptr = word_value; fill_ptr++;}

/* RECORD... macros record the results of modeling in the intermediate
 * arrays
 */

#define RECORD_ZERO { EMIT_BYTE(next_tag,ZERO_TAG); }

#define RECORD_EXACT(queue_posn)  EMIT_BYTE(next_tag,EXACT_TAG);  \
                                  EMIT_BYTE(next_qp,(queue_posn)); 

#define RECORD_PARTIAL(queue_posn,low_bits_pattern) { \
   EMIT_BYTE(next_tag,PARTIAL_TAG);                   \
   EMIT_BYTE(next_qp,(queue_posn));                   \
   EMIT_WORD(next_low_bits,(low_bits_pattern))  }

#define RECORD_MISS(word_pattern) EMIT_BYTE(next_tag,MISS_TAG); \
                                  EMIT_WORD(next_full_patt,(word_pattern)); 

/***********************************************************************
 *                   THE PACKING ROUTINES
 */

/* WK_pack_2bits()
 * Pack some multiple of four words holding two-bit tags (in the low
 * two bits of each byte) into an integral number of words, i.e.,
 * one fourth as many.  
 * NOTE: Pad the input out with zeroes to a multiple of four words!
 */
WK_word*
WK_pack_2bits(WK_word* source_buf,
              WK_word* source_end,
	      WK_word* dest_buf) {
   register WK_word* src_next = source_buf;
   WK_word* dest_next = dest_buf;
  
   while (src_next < source_end) {
      register WK_word temp = src_next[0];
      temp |= (src_next[1] << 2);
      temp |= (src_next[2] << 4);
      temp |= (src_next[3] << 6);
    
      dest_next[0] = temp;
      dest_next++;     
      src_next += 4;
   }
  
   return dest_next;

}

/* WK_pack_4bits()
 * Pack an even number of words holding 4-bit patterns in the low bits
 * of each byte into half as many words.
 * note: pad out the input with zeroes to an even number of words!
 */

WK_word*
WK_pack_4bits(WK_word* source_buf,
	      WK_word* source_end,
	      WK_word* dest_buf) {
   register WK_word* src_next = source_buf;
   WK_word* dest_next = dest_buf;
  
   /* this loop should probably be unrolled */
   while (src_next < source_end) {
     register WK_word temp = src_next[0];
     temp |= (src_next[1] << 4);
    
     dest_next[0] = temp;
     dest_next++;     
     src_next += 2;
   }

   return dest_next;

}

/* pack_3_tenbits()
 * Pack a sequence of three ten bit items into one word.
 * note: pad out the input with zeroes to an even number of words!
 */
WK_word*
WK_pack_3_tenbits(WK_word* source_buf,
		  WK_word* source_end,
		  WK_word* dest_buf) {

   register WK_word* src_next = source_buf;
   WK_word* dest_next = dest_buf;
  
   /* this loop should probably be unrolled */
   while (src_next < source_end) {
      register WK_word temp = src_next[0];
      temp |= (src_next[1] << 10);
      temp |= (src_next[2] << 20);
    
      dest_next[0] = temp;
      dest_next++;     
      src_next += 3;
   }

   return dest_next;

}

/***************************************************************************
 *          THE UNPACKING ROUTINES should GO HERE
 */


#define GET_NEXT_TAG tags[tagsIndex++]
#define GET_NEXT_FULL_PATTERN fullPatterns[fullPatternsIndex++]
#define GET_NEXT_LOW_BITS lowBits[lowBitsIndex++]
#define GET_NEXT_DICTIONARY_INDEX dictionaryIndices[dictionaryIndicesIndex++]

/*  WK_unpack_2bits takes any number of words containing 16 two-bit values
 *  and unpacks them into four times as many words containg those
 *  two bit values as bytes (with the low two bits of each byte holding
 *  the actual value.
 */
WK_word*
WK_unpack_2bits(WK_word *input_buf,
                WK_word *input_end,
                WK_word *output_buf) {

  register WK_word *input_next = input_buf;
  register WK_word *output_next = output_buf;
  register WK_word packing_mask = TWO_BITS_PACKING_MASK;

  /* loop to repeatedly grab one input word and unpack it into
   * 4 output words.  This loop could be unrolled a little---it's
   * designed to be easy to do that.
   */   
  while (input_next < input_end) {
    register WK_word temp = input_next[0];
    DEBUG_PRINT_2("Unpacked tags word: %.8x\n", temp);
    output_next[0] = temp & packing_mask;
    output_next[1] = (temp >> 2) & packing_mask;
    output_next[2] = (temp >> 4) & packing_mask;
    output_next[3] = (temp >> 6) & packing_mask;
    
    output_next += 4;
    input_next++;
  }

  return output_next;

}

/* unpack four bits consumes any number of words (between input_buf
 * and input_end) holding 8 4-bit values per word, and unpacks them
 * into twice as many words, with each value in a separate byte.
 * (The four-bit values occupy the low halves of the bytes in the
 * result).
 */
WK_word*
WK_unpack_4bits(WK_word *input_buf,
                WK_word *input_end,
                WK_word *output_buf) {

  register WK_word *input_next = input_buf;
  register WK_word *output_next = output_buf;
  register WK_word packing_mask = FOUR_BITS_PACKING_MASK;
  
  
  /* loop to repeatedly grab one input word and unpack it into
   * 4 output words.  This loop should probably be unrolled
   * a little---it's designed to be easy to do that.
   */   
  while (input_next < input_end) {
    register WK_word temp = input_next[0];
    DEBUG_PRINT_2("Unpacked dictionary indices word: %.8x\n", temp);
    output_next[0] = temp & packing_mask;
    output_next[1] = (temp >> 4) & packing_mask;
    
    output_next += 2;
    input_next++;
  }
  
  return output_next;

}

/* unpack_3_tenbits unpacks three 10-bit items from (the low 30 bits of)
 * a 32-bit word
 */
WK_word*
WK_unpack_3_tenbits(WK_word *input_buf,
                    WK_word *input_end,
                    WK_word *output_buf) {

  register WK_word *input_next = input_buf;
  register WK_word *output_next = output_buf;
  register WK_word packing_mask = LOW_BITS_MASK;
  
  /* loop to fetch words of input, splitting each into three
   * words of output with 10 meaningful low bits.  This loop
   * probably ought to be unrolled and maybe coiled
   */
  while (input_next < input_end) {
    register WK_word temp = input_next[0];
    
    output_next[0] = temp & LOW_BITS_MASK;
    output_next[1] = (temp >> 10) & LOW_BITS_MASK;
    output_next[2] = temp >> 20;
    
    input_next++;
    output_next += 3;
  }
  
  return output_next;

}
/***************************************************************************
 *  WKdm_compress()---THE COMPRESSOR
 */

unsigned int
WKdm_compress (WK_word* src_buf,
               WK_word* dest_buf,
	       unsigned int num_input_words)
{
  DictionaryElement dictionary[DICTIONARY_SIZE];
  char hashLookupTable [] = HASH_LOOKUP_TABLE_CONTENTS;

  /* arrays that hold output data in intermediate form during modeling */
  /* and whose contents are packed into the actual output after modeling */

  /* sizes of these arrays should be increased if you want to compress
   * pages larger than 4KB
   */
  WK_word tempTagsArray[300];         /* tags for everything          */
  WK_word tempQPosArray[300];         /* queue positions for matches  */
  WK_word tempLowBitsArray[1200];     /* low bits for partial matches */

  /* boundary_tmp will be used for keeping track of what's where in
   * the compressed page during packing
   */
  WK_word* boundary_tmp;

  /* Fill pointers for filling intermediate arrays (of queue positions
   * and low bits) during encoding.
   * Full words go straight to the destination buffer area reserved
   * for them.  (Right after where the tags go.)
   */
  WK_word* next_full_patt;
  char* next_tag = (char *) tempTagsArray;
  char* next_qp = (char *) tempQPosArray;
  WK_word* next_low_bits = tempLowBitsArray;

  WK_word* next_input_word = src_buf;
  WK_word* end_of_input = src_buf + num_input_words;

  PRELOAD_DICTIONARY;

  next_full_patt = dest_buf + TAGS_AREA_OFFSET + (num_input_words / 16);

#ifdef WK_DEBUG
  printf("\nIn WKdm_compress\n");
  printf("About to actually compress, src_buf is %u\n", src_buf);
  printf("dictionary is at %u\n", dictionary);
  printf("dest_buf is %u next_full_patt is %u\n", dest_buf, next_full_patt);
  fflush(stdout);
#endif

  while (next_input_word < end_of_input)
  {
     WK_word *dict_location;
     WK_word dict_word;
     WK_word input_word = *next_input_word;

     /* compute hash value, which is a byte offset into the dictionary,
      * and add it to the base address of the dictionary. Cast back and
      * forth to/from char * so no shifts are needed
      */
     dict_location =
       (WK_word *)
       (((char*) dictionary) + HASH_TO_DICT_BYTE_OFFSET(input_word));

     dict_word = *dict_location;

     if (input_word == dict_word)
     {
        RECORD_EXACT(dict_location - dictionary); 
     }
     else if (input_word == 0) {
        RECORD_ZERO;
     }
     else
     {
        WK_word input_high_bits = HIGH_BITS(input_word);
        if (input_high_bits == HIGH_BITS(dict_word)) {
	  RECORD_PARTIAL(dict_location - dictionary, LOW_BITS(input_word));
          *dict_location = input_word;
        }
        else {
	  RECORD_MISS(input_word);
            *dict_location = input_word;
        }
     }
     next_input_word++;
  }

#ifdef WK_DEBUG
  printf("AFTER MODELING in WKdm_compress()\n");  fflush(stdout);
  printf("tempTagsArray holds %u bytes\n",
         next_tag - (char *) tempTagsArray);
  printf("tempQPosArray holds %u bytes\n",
         next_qp - (char *) tempQPosArray);
  printf("tempLowBitsArray holds %u bytes\n",
         (char *) next_low_bits - (char *) tempLowBitsArray);

  printf("next_full_patt is %u\n",
         (unsigned long) next_full_patt);

  printf(" i.e., there are %u full patterns\n",
     next_full_patt - (dest_buf + TAGS_AREA_OFFSET + (num_input_words / 16)));
  fflush(stdout);

  { int i;
    WK_word *arr =(dest_buf + TAGS_AREA_OFFSET + (num_input_words / 16));

    printf("  first 20 full patterns are: \n");
    for (i = 0; i < 20; i++) {
      printf(" %d", arr[i]);
    }
    printf("\n");
  }
#endif

  /* Record (into the header) where we stopped writing full words,
   * which is where we will pack the queue positions.  (Recall
   * that we wrote the full words directly into the dest buffer
   * during modeling.
   */

  SET_QPOS_AREA_START(dest_buf,next_full_patt);

  /* Pack the tags into the tags area, between the page header
   * and the full words area.  We don't pad for the packer
   * because we assume that the page size is a multiple of 16.
   */     

#ifdef WK_DEBUG
  printf("about to pack %u bytes holding tags\n", 
         next_tag - (char *) tempTagsArray);

  { int i;
    char* arr = (char *) tempTagsArray;

    printf("  first 200 tags are: \n");
    for (i = 0; i < 200; i++) {
      printf(" %d", arr[i]);
    }
    printf("\n");
  }
#endif

  boundary_tmp = WK_pack_2bits(tempTagsArray,
		               (WK_word *) next_tag,
			       dest_buf + HEADER_SIZE_IN_WORDS);

#ifdef WK_DEBUG  
    printf("packing tags stopped at %u\n", boundary_tmp);
#endif
  
  /* Pack the queue positions into the area just after
   * the full words.  We have to round up the source
   * region to a multiple of two words.
   */

  {
    unsigned int num_bytes_to_pack = next_qp - (char *) tempQPosArray;
    unsigned int num_packed_words = ceil((double) num_bytes_to_pack / 8);
    unsigned int num_source_words = num_packed_words * 2;
    WK_word* endQPosArray = tempQPosArray + num_source_words;

    /* Pad out the array with zeros to avoid corrupting real packed
       values. */
    for (; /* next_qp is already set as desired */
	 next_qp < (char*)endQPosArray;
	 next_qp++) {
      *next_qp = 0;
    }

#ifdef WK_DEBUG    
    printf("about to pack %u (bytes holding) queue posns.\n",
           num_bytes_to_pack);
    printf("packing them from %u words into %u words\n",
           num_source_words, num_packed_words);
    printf("dest is range %u to %u\n",
           next_full_patt, next_full_patt + num_packed_words);
    { int i;
      char *arr = (char *) tempQPosArray;
      printf("  first 200 queue positions are: \n");
      for (i = 0; i < 200; i++) {
        printf(" %d", arr[i]);
      }
      printf("\n");
    }
#endif
    
    boundary_tmp = WK_pack_4bits(tempQPosArray,
			         endQPosArray,
				 next_full_patt);
#ifdef WK_DEBUG
     printf("Packing of queue positions stopped at %u\n", boundary_tmp);
#endif 
//WK_DEBUG

    /* Record (into the header) where we stopped packing queue positions,
     * which is where we will start packing low bits.
     */
    SET_LOW_BITS_AREA_START(dest_buf,boundary_tmp);

  }

  /* Pack the low bit patterns into the area just after
   * the queue positions.  We have to round up the source
   * region to a multiple of three words.
   */

  {
    unsigned int num_tenbits_to_pack =
      next_low_bits - tempLowBitsArray;
    unsigned int num_packed_words = ceil((double) num_tenbits_to_pack / 3);
    unsigned int num_source_words = num_packed_words * 3;
    WK_word* endLowBitsArray = tempLowBitsArray + num_source_words;

    /* Pad out the array with zeros to avoid corrupting real packed
       values. */

    for (; /* next_low_bits is already set as desired */
	 next_low_bits < endLowBitsArray;
	 next_low_bits++) {
      *next_low_bits = 0;
    }

#ifdef WK_DEBUG
	  printf("about to pack low bits\n");
          printf("num_tenbits_to_pack is %u\n", num_tenbits_to_pack);
          printf("endLowBitsArray is %u\n", endLowBitsArray);
#endif
    
    boundary_tmp = WK_pack_3_tenbits (tempLowBitsArray,
		                      endLowBitsArray,
				      boundary_tmp);

    SET_LOW_BITS_AREA_END(dest_buf,boundary_tmp);

  }

  return ((char *) boundary_tmp - (char *) dest_buf);

} 

/*********************************************************************
 * WKdm_decompress --- THE DECOMPRESSOR                                 
 * Expects WORD pointers to the source and destination buffers
 * and a page size in words.  The page size had better be 1024 unless     
 * somebody finds the places that are dependent on the page size and 
 * fixes them
 */

unsigned long int
WKdm_decompress (WK_word* src_buf,
		 WK_word* dest_buf,
		 unsigned int words) {

  DictionaryElement dictionary[DICTIONARY_SIZE];
  unsigned int hashLookupTable [] = HASH_LOOKUP_TABLE_CONTENTS;

  /* arrays that hold output data in intermediate form during modeling */
  /* and whose contents are packed into the actual output after modeling */

  /* sizes of these arrays should be increased if you want to compress
   * pages larger than 4KB
   */
  WK_word tempTagsArray[300];        /* tags for everything          */
  WK_word tempQPosArray[300];        /* queue positions for matches  */
  WK_word tempLowBitsArray[1200];    /* low bits for partial matches */

  PRELOAD_DICTIONARY;

#ifdef WK_DEBUG
  printf("\nIn DECOMPRESSOR\n");
  printf("tempTagsArray is at %u\n", (unsigned long int) tempTagsArray);
  printf("tempQPosArray is at %u\n", (unsigned long int) tempQPosArray);
  printf("tempLowBitsArray is at %u\n", (unsigned long int) tempLowBitsArray);

  printf(" first four words of source buffer are:\n");
  printf("   %u\n   %u\n   %u\n   %u\n",
         src_buf[0], src_buf[1], src_buf[2], src_buf[3]);
  
  { int i;
    WK_word *arr =(src_buf + TAGS_AREA_OFFSET + (PAGE_SIZE_IN_WORDS / 16));

    printf("  first 20 full patterns are: \n");
    for (i = 0; i < 20; i++) {
      printf(" %d", arr[i]);
    }
    printf("\n");
  }
#endif

  WK_unpack_2bits(TAGS_AREA_START(src_buf),
                  TAGS_AREA_END(src_buf),
                  tempTagsArray);

#ifdef WK_DEBUG
  { int i;
    char* arr = (char *) tempTagsArray;

    printf("  first 200 tags are: \n");
    for (i = 0; i < 200; i++) {
      printf(" %d", arr[i]);
    }
    printf("\n");
  }
#endif

  WK_unpack_4bits(QPOS_AREA_START(src_buf),
                  QPOS_AREA_END(src_buf),
                  tempQPosArray);

#ifdef WK_DEBUG
  { int i;
    char* arr = (char *) tempQPosArray;

    printf("  first 200 queue positions are: \n");
    for (i = 0; i < 200; i++) {
      printf(" %d", arr[i]);
    }
    printf("\n");
  }
#endif

  WK_unpack_3_tenbits(LOW_BITS_AREA_START(src_buf),
                      LOW_BITS_AREA_END(src_buf),
                      tempLowBitsArray);

#ifdef WK_DEBUG
  printf("AFTER UNPACKING, about to enter main block \n");
#endif

  {
    register char *next_tag = (char *) tempTagsArray;
    char *tags_area_end =
       ((char *) tempTagsArray) + PAGE_SIZE_IN_WORDS;
    char *next_q_pos = (char *) tempQPosArray;
    WK_word *next_low_bits = tempLowBitsArray;
    WK_word *next_full_word = FULL_WORD_AREA_START(src_buf);

    WK_word *next_output = dest_buf;

#ifdef WK_DEBUG
    printf("next_output is %u\n", next_output);

    printf("next_tag is %u \n", next_tag);
    printf("tags_area_end is %u\n", tags_area_end);
    printf("next_q_pos is %u\n", next_q_pos);
    printf("next_low_bits is %u\n", next_low_bits);
    printf("next_full_word is %u\n", next_full_word);
#endif 

    /* this loop should probably be unrolled. Maybe we should unpack
     * as 4 bit values, giving two consecutive tags, and switch on
     * that 16 ways to decompress 2 words at a whack
     */
    while (next_tag < tags_area_end) {

       char tag = next_tag[0];

       switch(tag) {

         case ZERO_TAG: {
            *next_output = 0;
            break;
         }
         case EXACT_TAG: {
            WK_word *dict_location = dictionary + *(next_q_pos++);
            /* no need to replace dict. entry if matched exactly */
            *next_output = *dict_location;
            break;
         }
         case PARTIAL_TAG: {
            WK_word *dict_location = dictionary + *(next_q_pos++);
            {
               WK_word temp = *dict_location;

               /* strip out low bits */
               temp = ((temp >> NUM_LOW_BITS) << NUM_LOW_BITS);

               /* add in stored low bits from temp array */
               temp = temp | *(next_low_bits++);

               *dict_location = temp;      /* replace old value in dict. */
               *next_output = temp;    /* and echo it to output */
            }
            break;
         }
         case MISS_TAG: {
            WK_word missed_word = *(next_full_word++);
            WK_word *dict_location = 
              (WK_word *)
              (((char *) dictionary) + HASH_TO_DICT_BYTE_OFFSET(missed_word));
            *dict_location = missed_word;
            *next_output = missed_word;
            break;
         }
       }
       next_tag++;
       next_output++;
    }

#ifdef WK_DEBUG        
    printf("AFTER DECOMPRESSING\n");
    printf("next_output is %u\n", (unsigned long int) next_output);
    printf("next_tag is %u\n", (unsigned long int) next_tag);
    printf("next_full_word is %u\n", (unsigned long int) next_full_word);
    printf("next_q_pos is %u\n", (unsigned long int) next_q_pos);
#endif
  }
}

#ifdef WK_DEBUG
main()
{

   WK_word* source_buf;
   WK_word* dest_buf;
   WK_word* udest_buf;

   int i;

   source_buf = (WK_word*) malloc(4096 * sizeof(WK_word));
   dest_buf = (WK_word*) malloc(4096 * sizeof(WK_word));
   udest_buf = (WK_word*) malloc(4096 * sizeof(WK_word));
   
   for (i = 0; i < 1024; i++)
   {
      source_buf[i] = i * 15;
   }

   source_buf[1025] = 99999;

   for (i = 0; i < 1024; i += 2)
   {
      source_buf[i] = i * 1024;
   }

   for (i = 0; i < 1024; i += 17)
   {
      source_buf[i] = 0;
   }

   for (i = 0; i < 1024; i++)
   {
      udest_buf[i] = 666;
   }

   udest_buf[1025] = 55555;

   printf("source_buf is %u, dest_buf is %u, udest_buf is %u\n",
          source_buf, dest_buf, udest_buf);

   printf("first 50 words of source_buf are:\n");
   for (i = 0; i < 50; i++)
     printf(" %d", source_buf[i]);

   i = WKdm_compress(source_buf,dest_buf,1024);
   printf("WKdm_compress returned %u\n", i);

   printf("redzone value at end of source buf (shd be 99999) is %u\n",
          source_buf[1025]);
   
   i = WKdm_decompress(dest_buf,udest_buf,1024);

   printf("WKdm_decompress returned %u\n", i);
   printf("redzone value at end of udest buf (shd be 55555) is %u\n",
          udest_buf[1025]);

   printf("first 50 words of udest_buf are:\n");
   for (i = 0; i < 50; i++)
     printf(" %d", udest_buf[i]);

   i = bcmp(source_buf, udest_buf, 100);

   printf("bcmp of orig. and compr'd/decompr'd copy (shd be 0) is %u\n", i);

}
#endif
