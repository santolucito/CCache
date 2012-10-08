# /* Windows 32 bit (LIB) - Microsoft 32-bit C/C++ Compiler 11.00
#  * Copyright (C) 1996, 1997, 1998 Markus F.X.J. Oberhumer
#  * type `make -f b/win32/mc110.mak'
#  */

override b_win32 = true
liblzo = lzo.lib
o = .obj
exe = .exe

CC = cl -nologo -ML
CFLAGS          = -Iinclude -O2 -GF -W3
LZO_COMPILE_C   = $(CC) $(CFLAGS) -c $(<bs)
LZO_COMPILE_ASM = $(CC) -c $(<bs)
LZO_ARLIB       = lib -nologo -out:$(@bs) @b\\win32\\mc110.rsp
LZO_LINK        = $(CC) -Fe$(@bs) $(^bs)

VPATH = .;src;ltest;examples;tests
include b/lzo.mk

