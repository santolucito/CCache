# /* DOS 16 bit - Microsoft C 7.0
#  * Copyright (C) 1996, 1997, 1998 Markus F.X.J. Oberhumer
#  * type `make -f b/dos16/mc70.mak'
#  */

override b_dos16 = true
liblzo = lzo.lib
o = .obj
exe = .exe

CC = cl -nologo -f- -AL
CFLAGS          = -Iinclude -O -G2 -W3
## CFLAGS         += -D__LZO_STRICT_16BIT
## CFLAGS         += -DLZO_DEBUG
LZO_COMPILE_C   = $(CC) $(CFLAGS) -c $(<bs)
LZO_COMPILE_ASM = $(CC) -c $(<bs)
LZO_ARLIB       = lib /nologo $(@bs) @b\\dos16\\mc70.rsp,nul
LZO_LINK        = $(CC) -Fe$(@bs) $(^bs)

VPATH = .;src;ltest;examples;tests
include b/lzo.mk

