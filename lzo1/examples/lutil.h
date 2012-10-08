/* lutil.h -- utilities

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


#include <lzoconf.h>

#include <stdio.h>
#include <stddef.h>
#if !defined(NO_STDLIB_H)
#  include <stdlib.h>
#endif
#include <string.h>
#include <ctype.h>
#if defined(HAVE_STAT)
#  include <sys/types.h>
#  include <sys/stat.h>
#endif


/* allow the example programs to work with previous versions */

#if (LZO_VERSION < 0x1030)
#  define LZO1X_1_MEM_COMPRESS	LZO1X_MEM_COMPRESS
#  define lzo_malloc(s)			((lzo_byte *) malloc(s))
#  define lzo_free(p)			((p) ? (void)free(p) : (void)0)
#endif

#if (LZO_VERSION < 0x1030)
#  define lzo_fread(f,b,s)      (fread(b,1,s,f))
#  define lzo_fwrite(f,b,s)     (fwrite(b,1,s,f))
#else
#  include <lzoutil.h>
#endif


#define HEAP_ALLOC(var,size) \
	long __LZO_MMODEL var [ ((size) + (sizeof(long) - 1)) / sizeof(long) ]


/*
vi:ts=4
*/

