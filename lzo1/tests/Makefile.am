## Process this file with automake to create Makefile.in

AUTOMAKE_OPTIONS = no-dependencies


INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/src
CFLAGS = @CFLAGS@ $(CFLAGS_O) $(CFLAGS_W)


noinst_PROGRAMS = align chksum promote sizes

align_LDADD = @liblzo@
chksum_LDADD = @liblzo@


@liblzo@:
	cd $(top_builddir)/src && $(MAKE) all

