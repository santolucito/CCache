/* lpack.c -- example program: a simple file packer

   This file is part of the LZO real-time data compression library.

   Copyright (C) 1998 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer

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
// NOTE: this is an example program, so do not use to backup your data
//
// This program lacks things like sophisticated file handling but is
// pretty complete regarding compression - it should provide a good
// starting point for adaption for you applications.
//
// Please study LZO.FAQ and simple.c first.
**************************************************************************/

#include <lzo1x.h>
#include "lutil.h"

static const char *progname = NULL;

static const lzo_uint block_size = (128*1024L);

static unsigned long total_in = 0;
static unsigned long total_out = 0;

/* magic file header for compressed files */
static const unsigned char magic[6] =
	{ 0x00, 0x4c, 0x5a, 0x4f, 0xff, 0x1a };


/*************************************************************************
// file IO
**************************************************************************/

lzo_uint xread(FILE *f, lzo_voidp buf, lzo_uint len, lzo_bool allow_eof)
{
	lzo_uint l;

	l = lzo_fread(f,buf,len);
	if (l > len)
	{
		fprintf(stderr,"\nsomething's wrong with your C library !!!\n");
		exit(1);
	}
	if (l != len && !allow_eof)
	{
		fprintf(stderr,"\nread error - premature end of file\n");
		exit(1);
	}
	total_in += l;
	return l;
}


lzo_uint xwrite(FILE *f, const lzo_voidp buf, lzo_uint len)
{
	if (f != NULL && lzo_fwrite(f,buf,len) != len)
	{
		fprintf(stderr,"\nwrite error  (disk full ?)\n");
		exit(1);
	}
	total_out += len;
	return len;
}


/* read and write portable 32-bit integers */

lzo_uint32 xread32(FILE *f)
{
	unsigned char b[4];
	lzo_uint32 v;

	xread(f,b,4,0);
	v  = (lzo_uint32) b[3] <<  0;
	v |= (lzo_uint32) b[2] <<  8;
	v |= (lzo_uint32) b[1] << 16;
	v |= (lzo_uint32) b[0] << 24;
	return v;
}


void xwrite32(FILE *f, lzo_uint32 v)
{
	unsigned char b[4];

	b[3] = (unsigned char) (v >>  0);
	b[2] = (unsigned char) (v >>  8);
	b[1] = (unsigned char) (v >> 16);
	b[0] = (unsigned char) (v >> 24);
	xwrite(f,b,4);
}


/*************************************************************************
// compress
//
// possible improvement: we could use overlapping compression to
//   save some memory - see overlap.c. This would require some minor
//   changes in the decompression code as well, because if a block
//   turns out to be incompressible we would still have to store it in its
//   compressed (slightly enlarged) form because the original (uncompressed)
//   data would have been lost during the overlapping compression.
**************************************************************************/

int do_compress(FILE *fi, FILE *fo)
{
	int r = 0;
	lzo_byte *in = NULL;
	lzo_byte *out = NULL;
	lzo_byte *wrkmem = NULL;
	lzo_uint in_len;
	lzo_uint out_len;
	lzo_uint32 flags = 0;		/* unused - could add some information here */
	lzo_uint32 checksum;

/*
 * Step 1: allocate compression buffers and work-memory
 */
	in = lzo_malloc(block_size);
	out = lzo_malloc(block_size + block_size / 64 + 16 + 3);
	wrkmem = lzo_malloc(LZO1X_1_MEM_COMPRESS);
	if (in == NULL || out == NULL || wrkmem == NULL)
	{
		printf("%s: out of memory\n", progname);
		r = 1;
		goto err;
	}

/*
 * Step 2: write magic header & flags, initialize checksum
 */
 	xwrite(fo,magic,sizeof(magic));
	xwrite32(fo,flags);
	checksum = lzo_adler32(0,NULL,0);

/*
 * Step 3: process blocks
 */
 	for (;;)
	{
		/* read block */
		in_len = xread(fi,in,block_size,1);
		if (in_len <= 0)
			break;

		/* update checksum */
		checksum = lzo_adler32(checksum,in,in_len);

		/* compress block */
		r = lzo1x_1_compress(in,in_len,out,&out_len,wrkmem);
		if (r != LZO_E_OK)
		{
			/* this should NEVER happen */
			printf("internal error - compression failed: %d\n", r);
			r = 2;
			goto err;
		}

		/* write uncompressed block size */
		xwrite32(fo,in_len);

		if (out_len < in_len)
		{
			/* write compressed block */
			xwrite32(fo,out_len);
			xwrite(fo,out,out_len);
		}
		else
		{
			/* not compressible - write uncompressed block */
			xwrite32(fo,in_len);
			xwrite(fo,in,in_len);
		}
	}

	/* write EOF marker */
	xwrite32(fo,0);

	/* write checksum */
	xwrite32(fo,checksum);

	r = 0;
err:
	lzo_free(wrkmem);
	lzo_free(out);
	lzo_free(in);
	return r;
}


/*************************************************************************
// decompress / test
//
// We are using overlapping (in-place) decompression to save some
// memory - see overlap.c.
**************************************************************************/

int do_decompress(FILE *fi, FILE *fo)
{
	int r = 0;
	lzo_byte *buf = NULL;
	lzo_uint buf_len;
	unsigned char m [ sizeof(magic) ];
	lzo_uint32 flags;
	lzo_uint32 checksum;

/*
 * Step 1: allocate buffer for in-place decompression
 */
	buf_len = block_size + block_size / 64 + 16 + 3;
	buf = lzo_malloc(buf_len);
	if (buf == NULL)
	{
		printf("%s: out of memory\n", progname);
		r = 1;
		goto err;
	}

/*
 * Step 2: check magic header, read flags, initialize checksum
 */
 	if (xread(fi,m,sizeof(magic),1) != sizeof(magic) ||
	    memcmp(m,magic,sizeof(magic)) != 0)
    {
		printf("%s: header error - this file is not compressed by lpack\n", progname);
		r = 2;
		goto err;
	}
	flags = xread32(fi);
	checksum = lzo_adler32(0,NULL,0);

/*
 * Step 3: process blocks
 */
 	for (;;)
	{
		lzo_byte *in;
		lzo_byte *out;
		lzo_uint in_len;
		lzo_uint out_len;

		/* read uncompressed size */
		out_len = xread32(fi);

		/* exit if last block (EOF marker) */
		if (out_len == 0)
			break;

		/* read compressed size */
		in_len = xread32(fi);

		/* sanity check of the size values */
		if (in_len > block_size || out_len > block_size ||
		    in_len == 0 || in_len > out_len)
		{
			printf("%s: block size error - data corrupted\n", progname);
			r = 3;
			goto err;
		}

		/* place compressed block at the top of the buffer */
 		in = buf + buf_len - in_len;
 		out = buf;

		/* read compressed block data */
		xread(fi,in,in_len,0);

		if (in_len < out_len)
		{
			/* decompress - use safe decompressor as data might be corrupted */
			lzo_uint new_len = out_len;

			r = lzo1x_decompress_safe(in,in_len,out,&new_len,NULL);
			if (r != LZO_E_OK || new_len != out_len)
			{
				printf("%s: compressed data violation\n", progname);
				r = 4;
				goto err;
			}
			/* write decompressed block */
			xwrite(fo,out,out_len);
			/* update checksum */
			checksum = lzo_adler32(checksum,out,out_len);
		}
		else
		{
			/* write original (incompressible) block */
			xwrite(fo,in,in_len);
			/* update checksum */
			checksum = lzo_adler32(checksum,in,in_len);
		}
	}

	/* read and verify checksum */
	if (xread32(fi) != checksum)
	{
		printf("%s: checksum error - data corrupted\n", progname);
		r = 5;
		goto err;
	}

	r = 0;
err:
	lzo_free(buf);
	return r;
}


/*************************************************************************
//
**************************************************************************/

int main(int argc, char *argv[])
{
	int i = 1;
	int r = 0;
	FILE *fi = NULL;
	FILE *fo = NULL;
	const char *in_name = NULL;
	const char *out_name = NULL;
	lzo_bool opt_decompress = 0;
	lzo_bool opt_test = 0;

#if defined(__EMX__)
	_response(&argc,&argv);
	_wildcard(&argc,&argv);
#endif

	printf("\nLZO real-time data compression library (v%s, %s).\n",
	        LZO_VERSION_STRING, LZO_VERSION_DATE);
	printf("Copyright (C) 1996, 1997, 1998 Markus Franz Xaver Johannes Oberhumer\n\n");

	printf(
"*** DISCLAIMER ***\n"
"   This is an example program, do not use to backup your data !\n"
"   Get LZOP if you're interested into a full-featured packer.\n"
"   See http://wildsau.idv.uni-linz.ac.at/mfx/lzop.html\n"
"\n");

	progname = argv[0];
	if (i < argc && strcmp(argv[i],"-d") == 0)
		opt_decompress = 1, i++;
	else if (i < argc && strcmp(argv[i],"-t") == 0)
		opt_test = 1, i++;
	if (i + (opt_test ? 1 : 2) != argc)
	{
		printf("usage: %s    input-file output-file  (compress)\n", progname);
		printf("usage: %s -d input-file output-file  (decompress)\n", progname);
		printf("usage: %s -t input-file              (test)\n", progname);
		exit(1);
	}

/*
 * Step 1: initialize the LZO library
 */
	if (lzo_init() != LZO_E_OK)
	{
		printf("lzo_init() failed !!!\n");
		exit(1);
	}

/*
 * Step 2: open the files
 */
 	in_name = argv[i++];
	fi = fopen(in_name,"rb");
	if (fi == NULL)
	{
		printf("%s: cannot open input file %s\n", progname, in_name);
		exit(1);
	}
#if defined(HAVE_STAT) && defined(S_ISREG)
	{
		struct stat st;
		if (stat(in_name,&st) != 0 || !S_ISREG(st.st_mode))
		{
			printf("%s: %s is not a regular file\n", progname, in_name);
			fclose(fi);
			exit(1);
		}
	}
#endif

	if (!opt_test)
	{
 		out_name = argv[i++];
		/* this is an example program, so make sure we don't overwrite a file */
		fo = fopen(out_name,"rb");
		if (fo != NULL)
		{
			printf("%s: file %s already exists -- not overwritten\n", progname, out_name);
			fclose(fi);
			fclose(fo);
			exit(1);
		}
		fo = fopen(out_name,"wb");
		if (fo == NULL)
		{
			printf("%s: cannot open output file %s\n", progname, out_name);
			fclose(fi);
			exit(1);
		}
	}

/*
 * Step 3: process file
 */
 	if (opt_test)
	{
		r = do_decompress(fi,NULL);
		if (r == 0)
			printf("%s: tested ok (%ld -> %ld bytes)\n",
			        progname, total_in, total_out);
	}
 	else if (opt_decompress)
	{
		r = do_decompress(fi,fo);
		if (r == 0)
			printf("%s: decompressed %ld into %ld bytes\n",
			        progname, total_in, total_out);
	}
	else
	{
		r = do_compress(fi,fo);
		if (r == 0)
			printf("%s: compressed %ld into %ld bytes\n",
			        progname, total_in, total_out);
	}

	fclose(fi);
	if (fo) fclose(fo);
	return r;
}

/*
vi:ts=4
*/

