## Process this file with automake to create Makefile.in

AUTOMAKE_OPTIONS = no-dependencies


INCLUDES = -I$(top_srcdir)/include
CFLAGS = @CFLAGS@ $(CFLAGS_O) $(CFLAGS_W)


noinst_PROGRAMS = dict lpack overlap precomp precomp2 simple

LDADD = @liblzo@

EXTRA_DIST = lutil.h


@liblzo@:
	cd $(top_builddir)/src && $(MAKE) all

