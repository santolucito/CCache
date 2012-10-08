## Process this file with automake to create Makefile.in

AUTOMAKE_OPTIONS = no-dependencies


### LZO_DEBUG = -DLZO_DEBUG
### LZO_DEBUG = -DLZO_DEBUG -g -fno-omit-frame-pointer

INCLUDES = -I$(top_srcdir)/include
CFLAGS = @CFLAGS@ $(CFLAGS_O) $(CFLAGS_W) $(LZO_DEBUG)


noinst_PROGRAMS = ltest

ltest_SOURCES = ltest.c asm.h db.h wrap.h timer.h mygetopt.ch mygetopt.h
ltest_LDADD = @liblzo@ $(ltest_LIBS)
ltest_DEPENDENCIES = @liblzo@


# /***********************************************************************
# // extra dependencies
# ************************************************************************/

@GMAKE_TRUE@ltest.o: $(top_srcdir)/include/lzoconf.h $(srcdir)/asm.h $(srcdir)/db.h $(srcdir)/timer.h $(srcdir)/wrap.h

@liblzo@:
	cd $(top_builddir)/src && $(MAKE) all

