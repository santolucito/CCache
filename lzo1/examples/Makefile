# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.2f from Makefile.am

# Copyright (C) 1994, 1995, 1996, 1997, 1998 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = .
top_srcdir = ..
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

pkgdatadir = $(datadir)/lzo
pkglibdir = $(libdir)/lzo
pkgincludedir = $(includedir)/lzo

top_builddir = ..

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /lusr/gnu/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = sparc-sun-solaris2.6
host_triplet = sparc-sun-solaris2.6
ARCHFLAGS = 
ASFLAGS =  -x assembler-with-cpp
CC = gcc
CFLAGS_GCC_OMIT_FRAME_POINTER = -fomit-frame-pointer
CFLAGS_O = -O2
CFLAGS_W = -Wall -Wcast-align -Wwrite-strings
CFLAGS_W_PEDANTIC = 
CLEANFILES = *.i
CPP = gcc -E
DISTCLEANFILES = *.bb *.bbg *.da *.gcov bb.out bbtrace* gmon.out
EXEEXT = 
LD = /usr/ccs/bin/ld
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
LZO_ASM_OBJECTS = 
LZO_CFLAGS = 
MAINT = #M#
MAKEINFO = makeinfo
M_ARCH = unknown
M_CC = GCC
M_CPU = sparc
M_VPATH = 
NASM = 
NM = /usr/ccs/bin/nm -p
PACKAGE = lzo
RANLIB = ranlib
VERSION = 1.04
liblzo = $(top_builddir)/src/liblzo.la
ltest_LIBS = 

AUTOMAKE_OPTIONS = no-dependencies

INCLUDES = -I$(top_srcdir)/include
CFLAGS =  $(CFLAGS_O) $(CFLAGS_W)

noinst_PROGRAMS = dict lpack overlap precomp precomp2 simple

LDADD = $(top_builddir)/src/liblzo.la

EXTRA_DIST = lutil.h
mkinstalldirs = $(SHELL) $(top_srcdir)/acconfig/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 
noinst_PROGRAMS =  dict$(EXEEXT) lpack$(EXEEXT) overlap$(EXEEXT) \
precomp$(EXEEXT) precomp2$(EXEEXT) simple$(EXEEXT)
PROGRAMS =  $(noinst_PROGRAMS)


DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I..
CPPFLAGS =  -DLZO_HAVE_CONFIG_H
LDFLAGS = 
LIBS = 
dict_SOURCES = dict.c
dict_OBJECTS =  dict.o
dict_LDADD = $(LDADD)
dict_DEPENDENCIES = 
dict_LDFLAGS = 
lpack_SOURCES = lpack.c
lpack_OBJECTS =  lpack.o
lpack_LDADD = $(LDADD)
lpack_DEPENDENCIES = 
lpack_LDFLAGS = 
overlap_SOURCES = overlap.c
overlap_OBJECTS =  overlap.o
overlap_LDADD = $(LDADD)
overlap_DEPENDENCIES = 
overlap_LDFLAGS = 
precomp_SOURCES = precomp.c
precomp_OBJECTS =  precomp.o
precomp_LDADD = $(LDADD)
precomp_DEPENDENCIES = 
precomp_LDFLAGS = 
precomp2_SOURCES = precomp2.c
precomp2_OBJECTS =  precomp2.o
precomp2_LDADD = $(LDADD)
precomp2_DEPENDENCIES = 
precomp2_LDFLAGS = 
simple_SOURCES = simple.c
simple_OBJECTS =  simple.o
simple_LDADD = $(LDADD)
simple_DEPENDENCIES = 
simple_LDFLAGS = 
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --mode=compile $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LINK = $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP = --best
SOURCES = dict.c lpack.c overlap.c precomp.c precomp2.c simple.c
OBJECTS = dict.o lpack.o overlap.o precomp.o precomp2.o simple.o

default: all

.SUFFIXES:
.SUFFIXES: .S .c .lo .o .s
$(srcdir)/Makefile.in: #M# Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu examples/Makefile

Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-noinstPROGRAMS:

clean-noinstPROGRAMS:
	-test -z "$(noinst_PROGRAMS)" || rm -f $(noinst_PROGRAMS)

distclean-noinstPROGRAMS:

maintainer-clean-noinstPROGRAMS:

.c.o:
	$(COMPILE) -c $<

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

.c.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

.s.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

.S.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:

maintainer-clean-libtool:

dict$(EXEEXT): $(dict_OBJECTS) $(dict_DEPENDENCIES)
	@rm -f dict$(EXEEXT)
	$(LINK) $(dict_LDFLAGS) $(dict_OBJECTS) $(dict_LDADD) $(LIBS)

lpack$(EXEEXT): $(lpack_OBJECTS) $(lpack_DEPENDENCIES)
	@rm -f lpack$(EXEEXT)
	$(LINK) $(lpack_LDFLAGS) $(lpack_OBJECTS) $(lpack_LDADD) $(LIBS)

overlap$(EXEEXT): $(overlap_OBJECTS) $(overlap_DEPENDENCIES)
	@rm -f overlap$(EXEEXT)
	$(LINK) $(overlap_LDFLAGS) $(overlap_OBJECTS) $(overlap_LDADD) $(LIBS)

precomp$(EXEEXT): $(precomp_OBJECTS) $(precomp_DEPENDENCIES)
	@rm -f precomp$(EXEEXT)
	$(LINK) $(precomp_LDFLAGS) $(precomp_OBJECTS) $(precomp_LDADD) $(LIBS)

precomp2$(EXEEXT): $(precomp2_OBJECTS) $(precomp2_DEPENDENCIES)
	@rm -f precomp2$(EXEEXT)
	$(LINK) $(precomp2_LDFLAGS) $(precomp2_OBJECTS) $(precomp2_LDADD) $(LIBS)

simple$(EXEEXT): $(simple_OBJECTS) $(simple_DEPENDENCIES)
	@rm -f simple$(EXEEXT)
	$(LINK) $(simple_LDFLAGS) $(simple_OBJECTS) $(simple_LDADD) $(LIBS)

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $(SOURCES) $(HEADERS) $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags  $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = examples

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  test -f $(distdir)/$$file \
	  || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	  || cp -p $$d/$$file $(distdir)/$$file; \
	done
info:
dvi:
check: all
	$(MAKE)
installcheck:
install-exec: 
	@$(NORMAL_INSTALL)

install-data: 
	@$(NORMAL_INSTALL)

install: install-exec install-data all
	@:

uninstall: 

all: Makefile $(PROGRAMS)

install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' INSTALL_SCRIPT='$(INSTALL_PROGRAM)' install
installdirs:


mostlyclean-generic:
	-test -z "$(MOSTLYCLEANFILES)" || rm -f $(MOSTLYCLEANFILES)

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-rm -f Makefile $(DISTCLEANFILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)

maintainer-clean-generic:
	-test -z "$(MAINTAINERCLEANFILES)" || rm -f $(MAINTAINERCLEANFILES)
	-test -z "$(BUILT_SOURCES)" || rm -f $(BUILT_SOURCES)
mostlyclean:  mostlyclean-noinstPROGRAMS mostlyclean-compile \
		mostlyclean-libtool mostlyclean-tags \
		mostlyclean-generic

clean:  clean-noinstPROGRAMS clean-compile clean-libtool clean-tags \
		clean-generic mostlyclean

distclean:  distclean-noinstPROGRAMS distclean-compile distclean-libtool \
		distclean-tags distclean-generic clean
	-rm -f config.status
	-rm -f libtool

maintainer-clean:  maintainer-clean-noinstPROGRAMS \
		maintainer-clean-compile maintainer-clean-libtool \
		maintainer-clean-tags maintainer-clean-generic \
		distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

.PHONY: default mostlyclean-noinstPROGRAMS distclean-noinstPROGRAMS \
clean-noinstPROGRAMS maintainer-clean-noinstPROGRAMS \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile mostlyclean-libtool distclean-libtool \
clean-libtool maintainer-clean-libtool tags mostlyclean-tags \
distclean-tags clean-tags maintainer-clean-tags distdir info dvi \
installcheck install-exec install-data install uninstall all \
installdirs mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


$(top_builddir)/src/liblzo.la:
	cd $(top_builddir)/src && $(MAKE) all

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
