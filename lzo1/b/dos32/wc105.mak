# /* DOS 32 bit - Watcom C 10.5
#  * Copyright (C) 1996, 1997, 1998 Markus F.X.J. Oberhumer
#  * type `make -f b/dos32/wc105.mak'
#  */

override b_dos32 = true
liblzo = lzo.lib
o = .obj
exe = .exe

CC = wcl386 -zq -mf -5r -bt=dos
CFLAGS          = -Iinclude -wx -oneatx
### CFLAGS         += -cc++
LZO_COMPILE_C   = $(CC) $(CFLAGS) -zc -c $(<bs)
LZO_COMPILE_ASM = $(CC) -wx -c $(<bs)
LZO_ARLIB       = wlib -q -b -n -t $(@bs) @b\\dos32\\wc105.rsp
LZO_LINK        = $(CC) -l=dos4g -fe=$(@bs) $(^bs)

liblzo_extra_SOURCES = $(LZO_ASM_SOURCES_WC_i386)

VPATH = .;src;src/i386/d_asm2;ltest;examples;tests
include b/lzo.mk

