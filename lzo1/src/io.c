/* io.c -- io functions

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


#include "lzo_conf.h"

#if !defined(NO_STDIO_H)
#include <stdio.h>
#include <lzoutil.h>

#undef lzo_fread
#undef lzo_fwrite


/***********************************************************************
//
************************************************************************/

LZO_PUBLIC(lzo_uint)
lzo_fread(FILE *f, lzo_voidp s, lzo_uint len)
{
#if 1 && (LZO_UINT_MAX <= SIZE_T_MAX)
	return fread(s,1,len,f);
#else
	lzo_byte *p = (lzo_byte *) s;
	lzo_uint l = 0;
	size_t k;
	unsigned char *b;
	unsigned char buf[512];

	while (l < len)
	{
		k = len - l > sizeof(buf) ? sizeof(buf) : (size_t) (len - l);
		k = fread(buf,1,k,f);
		if (k <= 0)
			break;
		l += k;
		b = buf; do *p++ = *b++; while (--k > 0);
	}
	return l;
#endif
}


/***********************************************************************
//
************************************************************************/

LZO_PUBLIC(lzo_uint)
lzo_fwrite(FILE *f, const lzo_voidp s, lzo_uint len)
{
#if 1 && (LZO_UINT_MAX <= SIZE_T_MAX)
	return fwrite(s,1,len,f);
#else
	const lzo_byte *p = (const lzo_byte *) s;
	lzo_uint l = 0;
	size_t k, n;
	unsigned char *b;
	unsigned char buf[512];

	while (l < len)
	{
		k = len - l > sizeof(buf) ? sizeof(buf) : (size_t) (len - l);
		b = buf; n = k; do *b++ = *p++; while (--n > 0);
		k = fwrite(buf,1,k,f);
		if (k <= 0)
			break;
		l += k;
	}
	return l;
#endif
}


#endif /* !defined(NO_STDIO_H) */


/*
vi:ts=4
*/
