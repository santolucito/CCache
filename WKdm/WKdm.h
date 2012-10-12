/* 
   WKdm-v0.1 -- Wilson-Kaplan-dm (direct-mapped) version 0.1
   
   Compresses buffers using a dictionary based match and partial match
   scheme.

   Paul Wilson -- wilson@cs.utexas.edu
   Scott F. Kaplan -- sfkaplan@cs.utexas.edu
   September 1997 */

/* ============================================================ */
/* Preprocessor directives to avoid multiple inclusion */
#if !defined (_WKdm_H)
#define _WKdm_H

/* ============================================================ */
/* Types and Constants */

/* A manipulable type that is the machine word size. */
typedef unsigned int WK_word;

#define PAGE_COMPRESS_WORDS_PER_PAGE 1024
#define DICTIONARY_SIZE 16

/* ============================================================ */
/* Interface */

/* If this header is being included into a C++ module, we need to be
   sure that the function names for this module don't suffer C++ name
   mangling. */
#if defined (__cplusplus)
extern "C" {
#endif /* __cplusplus */
  
/* Given pointers to source buffer (sourceBuffer) of uncompressed data
   and a destination buffer (destinationBuffer) into which compressed
   data can be placed, as well as the number of words in the source
   buffer (words), compress the contents of the source and place the
   results in the destination.  Return the number of bytes placed into
   the destination. */
unsigned int
WKdm_compress (WK_word* sourceBuffer,
	       WK_word* destinationBuffer,
	       unsigned int words);
  
/* Given a pointer to a source buffer (sourceBuffer) of compressed
   data and a pointer to a destination buffer (destinationBuffer) into
   which uncompressed data can be placed, as well as the number of
   uncompressed words that will be written to the destination buffer
   (words), decompress the data into the destination buffer. */
void
WKdm_decompress (WK_word* sourceBuffer,
		 WK_word* destinationPage,
		 unsigned int words);
  
/* Given a pointer to a source buffer (sourceBuffer) of uncompressed
   data, the number of words stored in that buffer (words), and two
   arrays for counting the number of exact (exactMatchCountArray) and
   partial (partialMatchCountArray) matches at each LRU position,
   compress the source and record what types of hits (partial or
   exact) occurred at each queue position.  Return the number of words
   that would be placed into a destination buffer (if there were
   one). */
unsigned int
WKdm_measureCompress (WK_word* sourceBuffer,
		     unsigned int words,
		     unsigned int* exactMatchCountArray,
		     unsigned int* partialMatchCountArray);

#if defined (__cplusplus)
}
#endif /* __cplusplus */

#endif /* _WKdm_H */
