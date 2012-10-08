#! /bin/sh -e

# /* PalmOS - m68k-palmos-coff-gcc 2.7.2.2-kgpd-071097
#  * a very simple make driver
#  * Copyright (C) 1997, 1998 Markus F.X.J. Oberhumer
#  */

CC="m68k-palmos-coff-gcc"
CFLAGS="-I. -Iinclude -O2 -Wall -W -DFILE=void"
OBJRES=m68k-palmos-coff-obj-res
PILRC=pilrc
BUILDPRC=build-prc
LIBS="-static liblzo.a -lc"

rm -f *.grc
$CC $CFLAGS -o simple.out b/palmos/simple.c $LIBS -Wl,-Map,simple.map
$OBJRES simple.out
$BUILDPRC simple.prc "LZO-simple" LZOa *.grc

rm -f *.grc
echo Done.
