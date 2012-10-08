# /* DOS 32 bit - Pentium gcc 2.7.2p snapshot 970725 (djgpp v2)
#  * Copyright (C) 1996, 1997, 1998 Markus F.X.J. Oberhumer
#  * type `make -f b/dos32/dj2_pgcc.mak'
#  *
#  * !!! pgcc is *EXPERIMENTAL* and may produce invalid code !!!
#  */

override b_dos32 = true
liblzo = liblzo.a
o = .o
exe = .exe

CC = pgcc
CFLAGS          = @b/gcc.opt @b/dos32/dj2.opt
ASFLAGS         =
COMPILE_C       = $(CC) $(CFLAGS) -c $<
LZO_COMPILE_C   = $(CC) $(CFLAGS) @b/gcc_lzo.opt @b/dos32/dj2_pgcc.opt -c $<
LZO_COMPILE_S   = $(CC) -x assembler-with-cpp -Wall $(ASFLAGS) -c $<
LZO_ARLIB       = ar rcs $@ $^
LZO_LINK        = $(CC) -s -o $@ $^
### LZO_LINK       += -Wl,-Map,$(basename $@).map

liblzo_extra_SOURCES = $(LZO_ASM_SOURCES_GCC_i386)

VPATH = .;src;src/i386/src;ltest;examples;tests
include b/lzo.mk

install: all
	cp -p include/lzo*.h $(DJDIR)/include/
	cp -p liblzo.a $(DJDIR)/lib/

