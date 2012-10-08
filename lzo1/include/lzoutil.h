/* lzoutil.h --

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
   <markus.oberhumer@jk.uni-linz.ac.at>
   http://wildsau.idv.uni-linz.ac.at/mfx/lzo.html
 */


#ifndef __LZOUTIL_H
#define __LZOUTIL_H

#ifndef __LZOCONF_H
#include <lzoconf.h>
#endif

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
//
************************************************************************/

LZO_EXTERN(lzo_uint)
lzo_fread(FILE *f, lzo_voidp buf, lzo_uint size);
LZO_EXTERN(lzo_uint)
lzo_fwrite(FILE *f, const lzo_voidp buf, lzo_uint size);


#if (LZO_UINT_MAX >= UINT_MAX)
   /* avoid problems with Win32 DLLs */
#  define lzo_fread(f,b,s)      (fread(b,1,s,f))
#  define lzo_fwrite(f,b,s)     (fwrite(b,1,s,f))
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* already included */

