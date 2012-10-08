#! /bin/sh -e

#
# usage: util/checkasm.sh [directory]
#
# This script runs ltest with all assembler decompressors
# on a complete directory tree.
# It is not suitable for accurate timings.
#
# Copyright (C) 1996, 1997, 1998 Markus Franz Xaver Johannes Oberhumer
#

LTEST="./ltest/ltest"
LFLAGS="-q"

dir="${*-.}"

TMPFILE="/tmp/lzo_$$.tmp"
rm -f $TMPFILE
(find $dir -type f -print > $TMPFILE) || true

for i in 11; do
	cat $TMPFILE | $LTEST -m${i} -@ $LFLAGS -A
	cat $TMPFILE | $LTEST -m${i} -@ $LFLAGS -A -S
done

for i in 61; do
	cat $TMPFILE | $LTEST -m${i} -@ $LFLAGS -F
	cat $TMPFILE | $LTEST -m${i} -@ $LFLAGS -F -S
done

for i in 71 81; do
	cat $TMPFILE | $LTEST -m${i} -@ $LFLAGS -A
	cat $TMPFILE | $LTEST -m${i} -@ $LFLAGS -A -S
	cat $TMPFILE | $LTEST -m${i} -@ $LFLAGS -F
	cat $TMPFILE | $LTEST -m${i} -@ $LFLAGS -F -S
done

rm -f $TMPFILE
echo "Done."
exit 0

