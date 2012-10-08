# /* Windows 32 bit (LIB) - Watcom C 10.5
#  * Copyright (C) 1996, 1997, 1998 Markus F.X.J. Oberhumer
#  * type `make -f b/win32/wc105.mak'
#  */

override b_win32 = true
liblzo = lzo.lib
o = .obj
exe = .exe

CC = wcl386 -zq -mf -5r -bt=nt
CFLAGS          = -Iinclude -wx -oneatx
LZO_COMPILE_C   = $(CC) $(CFLAGS) -zc -c $(<bs)
LZO_COMPILE_ASM = $(CC) -wx -c $(<bs)
LZO_ARLIB       = wlib -q -b -n -t $(@bs) @b\\win32\\wc105.rsp
LZO_LINK        = $(CC) -l=nt -fe=$(@bs) $(^bs)

liblzo_extra_SOURCES = $(LZO_ASM_SOURCES_WC_i386)

VPATH = .;src;src/i386/d_asm2;ltest;examples;tests
include b/lzo.mk

