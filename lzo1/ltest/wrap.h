/* wrap.h -- wrapper functions

   This file is part of the LZO real-time data compression library.

   Copyright (C) 1998 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer

   The LZO library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The LZO library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the LZO library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   Markus F.X.J. Oberhumer
   markus.oberhumer@jk.uni-linz.ac.at
 */


/*************************************************************************
// compression levels of LZO1X-999 and LZO1Y-999
**************************************************************************/

#if defined(HAVE_LZO1X_H) && !defined(LZO_999_UNSUPPORTED)

LZO_PRIVATE(int)
lzo1x_999_1_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1x_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 1);
}

LZO_PRIVATE(int)
lzo1x_999_2_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1x_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 2);
}

LZO_PRIVATE(int)
lzo1x_999_3_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1x_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 3);
}

LZO_PRIVATE(int)
lzo1x_999_4_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1x_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 4);
}

LZO_PRIVATE(int)
lzo1x_999_5_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1x_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 5);
}

LZO_PRIVATE(int)
lzo1x_999_6_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1x_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 6);
}

LZO_PRIVATE(int)
lzo1x_999_7_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1x_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 7);
}

LZO_PRIVATE(int)
lzo1x_999_8_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1x_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 8);
}

LZO_PRIVATE(int)
lzo1x_999_9_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1x_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 9);
}

#endif


#if defined(HAVE_LZO1Y_H) && !defined(LZO_999_UNSUPPORTED)

LZO_PRIVATE(int)
lzo1y_999_1_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1y_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 1);
}

LZO_PRIVATE(int)
lzo1y_999_2_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1y_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 2);
}

LZO_PRIVATE(int)
lzo1y_999_3_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1y_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 3);
}

LZO_PRIVATE(int)
lzo1y_999_4_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1y_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 4);
}

LZO_PRIVATE(int)
lzo1y_999_5_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1y_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 5);
}

LZO_PRIVATE(int)
lzo1y_999_6_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1y_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 6);
}

LZO_PRIVATE(int)
lzo1y_999_7_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1y_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 7);
}

LZO_PRIVATE(int)
lzo1y_999_8_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1y_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 8);
}

LZO_PRIVATE(int)
lzo1y_999_9_compress    ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	return lzo1y_999_compress_level(src, src_len, dst, dst_len, wrkmem,
	                                dict, dict_len, 0, 9);
}

#endif


/*************************************************************************
// zlib wrapper
**************************************************************************/

#if defined(HAVE_ZLIB_H)

#if (ULONG_MAX > 0xffffffffL)		/* 64 bit or more */
#define ZLIB_MEM_COMPRESS		600000l
#define ZLIB_MEM_DECOMPRESS		120000l
#else
#define ZLIB_MEM_COMPRESS		300000l
#define ZLIB_MEM_DECOMPRESS		 60000l
#endif

static lzo_byte *zlib_heap_ptr = NULL;
static lzo_uint32 zlib_heap_used = 0;
static lzo_uint32 zlib_heap_size = WRK_LEN;

static
voidpf zlib_zalloc ( voidpf opaque, unsigned items, unsigned size )
{
	lzo_uint32 bytes = (lzo_uint32) items * size;
	voidpf ptr = (voidpf) zlib_heap_ptr;

	bytes = (bytes + 15) & ~15;
	if (zlib_heap_used + bytes > zlib_heap_size)
		return 0;

	zlib_heap_ptr  += bytes;
	zlib_heap_used += bytes;
	opaque = 0;
	return ptr;
}

static
void zlib_zfree ( voidpf opaque, voidpf ptr )
{
	opaque = 0;
	ptr = 0;
}

static
void zlib_alloc_init ( z_stream *strm, lzo_bytep wrkmem )
{
    zlib_heap_ptr  = wrkmem;
    zlib_heap_used = 0;

	strm->zalloc = (alloc_func)zlib_zalloc;
	strm->zfree = (free_func)zlib_zfree;
}


int zlib_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem,
								int c, int level )
{
	/* use the undocumented feature to suppress the zlib header */
	z_stream stream;
	int err;
	int windowBits = opt_dict ? MAX_WBITS : -MAX_WBITS;

	stream.next_in = (lzo_byte *) src;		/* UNCONST */
	stream.avail_in = src_len;
	stream.next_out = dst;
	stream.avail_out = *dst_len;
	*dst_len = 0;

	zlib_alloc_init(&stream,wrkmem);

#if 0
	err = deflateInit(&stream, level);
#else
	err = deflateInit2(&stream, level, c, windowBits,
	                   MAX_MEM_LEVEL > 8 ? 8 : MAX_MEM_LEVEL,
	                   Z_DEFAULT_STRATEGY);
#endif
	if (err == Z_OK && opt_dict && dict)
		err = deflateSetDictionary(&stream,dict,dict_len);
	if (err == Z_OK)
	{

		err = deflate(&stream, Z_FINISH);
		if (err != Z_STREAM_END)
		{
			deflateEnd(&stream);
			err = (err == Z_OK) ? Z_BUF_ERROR : err;
		}
		else
		{
			*dst_len = (lzo_uint) stream.total_out;
			err = deflateEnd(&stream);
		}
	}
	return err;
}


LZO_PRIVATE(int)
zlib_decompress         ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	/* use the undocumented feature to suppress the zlib header */
	z_stream stream;
	int err;
	int windowBits = opt_dict ? MAX_WBITS : -MAX_WBITS;

	stream.next_in = (lzo_byte *) src;		/* UNCONST */
	stream.avail_in = src_len;
	stream.next_out = dst;
	stream.avail_out = *dst_len;
	*dst_len = 0;

	zlib_alloc_init(&stream,wrkmem);

#if 0
	err = inflateInit(&stream);
#else
	err = inflateInit2(&stream, windowBits);
#endif
	while (err == Z_OK)
	{
		err = inflate(&stream, Z_FINISH);
		if (err == Z_OK)		/* sometimes returns Z_OK !!! */
		{
			*dst_len = (lzo_uint) stream.total_out;
			err = inflateEnd(&stream);
			break;
		}
		else if (err == Z_STREAM_END)
		{
			*dst_len = (lzo_uint) stream.total_out;
			err = inflateEnd(&stream);
			break;
		}
		else if (err == Z_NEED_DICT && opt_dict && dict)
			err = inflateSetDictionary(&stream,dict,dict_len);
		else
			(void) inflateEnd(&stream);
	}
	return err;
}


LZO_PRIVATE(int)
zlib_8_1_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{ return zlib_compress(src,src_len,dst,dst_len,wrkmem,Z_DEFLATED,1); }

LZO_PRIVATE(int)
zlib_8_2_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{ return zlib_compress(src,src_len,dst,dst_len,wrkmem,Z_DEFLATED,2); }

LZO_PRIVATE(int)
zlib_8_3_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{ return zlib_compress(src,src_len,dst,dst_len,wrkmem,Z_DEFLATED,3); }

LZO_PRIVATE(int)
zlib_8_4_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{ return zlib_compress(src,src_len,dst,dst_len,wrkmem,Z_DEFLATED,4); }

LZO_PRIVATE(int)
zlib_8_5_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{ return zlib_compress(src,src_len,dst,dst_len,wrkmem,Z_DEFLATED,5); }

LZO_PRIVATE(int)
zlib_8_6_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{ return zlib_compress(src,src_len,dst,dst_len,wrkmem,Z_DEFLATED,6); }

LZO_PRIVATE(int)
zlib_8_7_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{ return zlib_compress(src,src_len,dst,dst_len,wrkmem,Z_DEFLATED,7); }

LZO_PRIVATE(int)
zlib_8_8_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{ return zlib_compress(src,src_len,dst,dst_len,wrkmem,Z_DEFLATED,8); }

LZO_PRIVATE(int)
zlib_8_9_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{ return zlib_compress(src,src_len,dst,dst_len,wrkmem,Z_DEFLATED,9); }


#endif /* HAVE_ZLIB_H */


/*************************************************************************
// other wrappers (pseudo compressors)
**************************************************************************/

LZO_PRIVATE(int)
memcpy_x_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	lzo_memcpy(dst,src,src_len);
	*dst_len = src_len;
	if (wrkmem) wrkmem = 0;	/* avoid warning */
	return 0;
}


LZO_PRIVATE(int)
memset_x_compress       ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	lzo_memset(dst,0,src_len);
	*dst_len = src_len;
	if (src) src = 0;		/* avoid warning */
	if (wrkmem) wrkmem = 0;	/* avoid warning */
	return 0;
}


LZO_PRIVATE(int)
adler32_x_compress      ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	lzo_uint32 adler;
	adler = lzo_adler32(0, NULL, 0);
	adler = lzo_adler32(adler, dst, src_len);
	*dst_len = src_len;
	if (src) src = 0;		/* avoid warning */
	if (wrkmem) wrkmem = 0;	/* avoid warning */
	return 0;
}


LZO_PRIVATE(int)
crc32_x_compress        ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	lzo_uint32 crc;
	crc = lzo_crc32(0, NULL, 0);
	crc = lzo_crc32(crc, dst, src_len);
	*dst_len = src_len;
	if (src) src = 0;		/* avoid warning */
	if (wrkmem) wrkmem = 0;	/* avoid warning */
	return 0;
}


#if defined(HAVE_ZLIB_H)

LZO_PRIVATE(int)
zlib_adler32_x_compress ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	uLong adler;
	adler = adler32(0L, Z_NULL, 0);
	adler = adler32(adler, dst, src_len);
	*dst_len = src_len;
	if (src) src = 0;		/* avoid warning */
	if (wrkmem) wrkmem = 0;	/* avoid warning */
	return 0;
}


LZO_PRIVATE(int)
zlib_crc32_x_compress   ( const lzo_byte *src, lzo_uint  src_len,
                                lzo_byte *dst, lzo_uint *dst_len,
                                lzo_voidp wrkmem )
{
	uLong crc;
	crc = crc32(0L, Z_NULL, 0);
	crc = crc32(crc, dst, src_len);
	*dst_len = src_len;
	if (src) src = 0;		/* avoid warning */
	if (wrkmem) wrkmem = 0;	/* avoid warning */
	return 0;
}

#endif


#if defined(MFX)
#  include "maint/t_wrap.ch"
#endif


/*
vi:ts=4
*/

