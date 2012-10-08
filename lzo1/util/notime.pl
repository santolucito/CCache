#! /usr/bin/perl
##
## vi:ts=4
##
##---------------------------------------------------------------------------##
##
##  Author:
##      Markus F.X.J. Oberhumer         markus.oberhumer@jk.uni-linz.ac.at
##
##  Description:
##      Remove timing values from a table created by table.pl
##
##  Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer
##  Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer
##
##---------------------------------------------------------------------------##


while (<>) {

	if (substr($_,52) =~ /^\s+[\d\.]+\s+[\d\.]+\s+\|\s*\n$/i) {
		substr($_,52) = "      0.00      0.00 |\n";
	}
	print;
}

exit(0);

