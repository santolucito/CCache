# /* DOS 32 bit - gcc 2.7.2.1 (emx 0.9c)
#  * Copyright (C) 1996, 1997, 1998 Markus F.X.J. Oberhumer
#  * type `make -f b/dos32/emx.mak'
#  */

export LFN = n

override b_dos32 = true
liblzo = lzo.a
o = .o
exe = .exe

CC = gcc
CFLAGS          = @b/gcc.opt @b/dos32/emx.opt
ASFLAGS         =
LZO_COMPILE_C   = $(CC) $(CFLAGS) @b/gcc_lzo.opt -c $<
LZO_COMPILE_S   = $(CC) -x assembler-with-cpp -Wall $(ASFLAGS) -c $<
LZO_ARLIB       = ar rcs $@ $^
LZO_LINK        = $(CC) -s -o $@ $^
### LZO_LINK       += -Wl,-Map,$(basename $@).map

liblzo_extra_SOURCES = $(LZO_ASM_SOURCES_GCC_i386)

VPATH = .;src;src/i386/src;ltest;examples;tests
include b/lzo.mk

