typedef unsigned char UBYTE;
typedef unsigned int UWORD;
typedef unsigned long ULONG;

#define FLAG_BYTES    4
#define FLAG_COMPRESS 0
#define FLAG_COPY     1

#if defined (__cplusplus)
extern "C" {
#endif

void fast_copy (UBYTE* p_src, UBYTE* p_dst, int len);

void lzrw1_compress (UBYTE* p_src_first,
		     ULONG src_len,
		     UBYTE* p_dst_first,
		     ULONG* p_dst_len);

void lzrw1_decompress(UBYTE* p_src_first,
		      ULONG src_len,
		      UBYTE* p_dst_first,
		      ULONG* p_dst_len);

#if defined (__cplusplus)
} /* extern "C" */
#endif
