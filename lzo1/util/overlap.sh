#! /bin/sh -e

#
# usage: util/overlap.sh [directory]
#
# This script runs the overlap example program on a complete directory tree.
#
# Copyright (C) 1997, 1998 Markus Franz Xaver Johannes Oberhumer
#

dir="${*-.}"

TMPFILE="/tmp/lzo_$$.tmp"
rm -f $TMPFILE
(find $dir -type f -print0 > $TMPFILE) || true

cat $TMPFILE | xargs -0 -r examples/overlap

rm -f $TMPFILE
echo "Done."
exit 0

