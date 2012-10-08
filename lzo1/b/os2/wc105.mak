# /* OS/2 32 bit - Watcom C 10.5
#  * Copyright (C) 1996, 1997, 1998 Markus F.X.J. Oberhumer
#  * type `make -f b/os2/wc105.mak'
#  */

override b_os2 = true
liblzo = lzo.lib
o = .obj
exe = .exe

CC = wcl386 -zq -mf -5r -bt=os2v2
CFLAGS          = -Iinclude -wx -oneatx
LZO_COMPILE_C   = $(CC) $(CFLAGS) -zc -c $(<bs)
LZO_COMPILE_ASM = $(CC) -wx -c $(<bs)
LZO_ARLIB       = wlib -q -b -n -t $(@bs) @b\\os2\\wc105.rsp
LZO_LINK        = $(CC) -l=os2v2 -fe=$(@bs) $(^bs)

liblzo_extra_SOURCES = $(LZO_ASM_SOURCES_WC_i386)

VPATH = .;src;src/i386/d_asm2;ltest;examples;tests
include b/lzo.mk

