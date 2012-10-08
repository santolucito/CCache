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

#
# Copyright (C) 1996, 1997, 1998 Markus F.X.J. Oberhumer
#


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

# /***********************************************************************
# // compiler and architecture specific CFLAGS
# ************************************************************************/

LZO_CFLAGS_GCC   =
LZO_CFLAGS_GCC_O =
LZO_CFLAGS_GCC_W = \
    -Wconversion -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes \
    -Wno-uninitialized

LZO_CFLAGS_GCC_i386   =
LZO_CFLAGS_GCC_i386_O = $(CFLAGS_GCC_OMIT_FRAME_POINTER) # -fforce-addr
LZO_CFLAGS_GCC_i386_W =

L_CFLAGS   = $(LZO_CFLAGS_GCC) $(LZO_CFLAGS_GCC_unknown)
L_CFLAGS_O = $(LZO_CFLAGS_GCC_O) $(LZO_CFLAGS_GCC_unknown_O)
L_CFLAGS_W = $(LZO_CFLAGS_GCC_W) $(LZO_CFLAGS_GCC_unknown_W)

# /***********************************************************************
# //
# ************************************************************************/

### LZO_DEBUG = -DLZO_DEBUG
### LZO_DEBUG = -DLZO_DEBUG -Wno-error
### LZO_DEBUG = -DLZO_DEBUG -g -fno-omit-frame-pointer -Wno-error
### LZO_EXTRA_CFLAGS = -fbranch-probabilities -Wno-error
### LZO_EXTRA_CFLAGS = -fprofile-arcs -ftest-coverage -O0

INCLUDES = -I$(top_srcdir)/include

CFLAGS =  $(L_CFLAGS) -O2 $(L_CFLAGS_O) -Wall -Wcast-align -Wwrite-strings $(L_CFLAGS_W)   $(LZO_EXTRA_CFLAGS) $(LZO_DEBUG)

VPATH = .:.

# /***********************************************************************
# // assembler support
# ************************************************************************/

SUFFIXES = .asm .c .i .s .sh

COMPILE_S   = $(CC) $(ASFLAGS) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(LZO_DEBUG)
COMPILE_ASM = $(NASM) -f elf

# /***********************************************************************
# // library files
# ************************************************************************/

lib_LTLIBRARIES = liblzo.la

liblzo_la_LDFLAGS = -version-info 1:0:0

liblzo_la_SOURCES = \
    lzo1.c lzo1_99.c \
    lzo1a.c lzo1a_99.c \
    lzo1b_1.c lzo1b_2.c lzo1b_3.c lzo1b_4.c lzo1b_5.c lzo1b_6.c \
    lzo1b_7.c lzo1b_8.c lzo1b_9.c lzo1b_99.c lzo1b_9x.c \
    lzo1b_cc.c lzo1b_d1.c lzo1b_d2.c lzo1b_rr.c lzo1b_xx.c \
    lzo1c_1.c lzo1c_2.c lzo1c_3.c lzo1c_4.c lzo1c_5.c lzo1c_6.c \
    lzo1c_7.c lzo1c_8.c lzo1c_9.c lzo1c_99.c lzo1c_9x.c \
    lzo1c_cc.c lzo1c_d1.c lzo1c_d2.c lzo1c_rr.c lzo1c_xx.c \
    lzo1f_1.c lzo1f_9x.c lzo1f_d1.c lzo1f_d2.c \
    lzo1x_1.c lzo1x_9x.c lzo1x_d1.c lzo1x_d2.c lzo1x_d3.c \
    lzo1x_o.c lzo1x_1k.c lzo1x_1l.c lzo1x_1o.c \
    lzo1y_1.c lzo1y_9x.c lzo1y_d1.c lzo1y_d2.c lzo1y_d3.c lzo1y_o.c \
    lzo1z_9x.c lzo1z_d1.c lzo1z_d2.c lzo1z_d3.c \
    lzo2a_9x.c lzo2a_d1.c lzo2a_d2.c \
     lzo1_cm.ch lzo1_d.ch lzo1a_cm.ch lzo1a_cr.ch lzo1b_c.ch \
     lzo1b_cm.ch lzo1b_cr.ch lzo1b_d.ch lzo1b_r.ch \
     lzo1b_sm.ch lzo1b_tm.ch lzo1f_d.ch lzo1x_c.ch \
     lzo1x_d.ch lzo1x_oo.ch lzo2a_d.ch lzo_mchw.ch lzo_swd.ch \
      alloc.c io.c lzo_crc.c lzo_dll.c lzo_init.c lzo_ptr.c lzo_str.c \
      lzo_util.c \
      compr1b.h compr1c.h config1.h config1a.h config1b.h config1c.h \
      config1f.h config1x.h config1y.h config1z.h config2a.h fake16.h \
      lzo1a_de.h lzo1b_cc.h lzo1b_de.h lzo1c_cc.h lzo_conf.h lzo_dict.h \
      lzo_ptr.h lzo_util.h stats1a.h stats1b.h stats1c.h

LZO_ASM_OBJECTS_GCC_i386 = \
    lzo1c_s1.lo lzo1c_s2.lo \
    lzo1f_f1.lo lzo1f_f2.lo \
    lzo1x_f1.lo lzo1x_f2.lo lzo1x_s1.lo lzo1x_s2.lo \
    lzo1y_f1.lo lzo1y_f2.lo lzo1y_s1.lo lzo1y_s2.lo

LZO_ASM_OBJECTS = 

liblzo_la_LIBADD = $(LZO_ASM_OBJECTS)
liblzo_la_DEPENDENCIES = $(liblzo_la_LIBADD)
mkinstalldirs = $(SHELL) $(top_srcdir)/acconfig/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 
LTLIBRARIES =  $(lib_LTLIBRARIES)


DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I..
CPPFLAGS =  -DLZO_HAVE_CONFIG_H
LDFLAGS = 
LIBS = 
liblzo_la_OBJECTS =  lzo1.lo lzo1_99.lo lzo1a.lo lzo1a_99.lo lzo1b_1.lo \
lzo1b_2.lo lzo1b_3.lo lzo1b_4.lo lzo1b_5.lo lzo1b_6.lo lzo1b_7.lo \
lzo1b_8.lo lzo1b_9.lo lzo1b_99.lo lzo1b_9x.lo lzo1b_cc.lo lzo1b_d1.lo \
lzo1b_d2.lo lzo1b_rr.lo lzo1b_xx.lo lzo1c_1.lo lzo1c_2.lo lzo1c_3.lo \
lzo1c_4.lo lzo1c_5.lo lzo1c_6.lo lzo1c_7.lo lzo1c_8.lo lzo1c_9.lo \
lzo1c_99.lo lzo1c_9x.lo lzo1c_cc.lo lzo1c_d1.lo lzo1c_d2.lo lzo1c_rr.lo \
lzo1c_xx.lo lzo1f_1.lo lzo1f_9x.lo lzo1f_d1.lo lzo1f_d2.lo lzo1x_1.lo \
lzo1x_9x.lo lzo1x_d1.lo lzo1x_d2.lo lzo1x_d3.lo lzo1x_o.lo lzo1x_1k.lo \
lzo1x_1l.lo lzo1x_1o.lo lzo1y_1.lo lzo1y_9x.lo lzo1y_d1.lo lzo1y_d2.lo \
lzo1y_d3.lo lzo1y_o.lo lzo1z_9x.lo lzo1z_d1.lo lzo1z_d2.lo lzo1z_d3.lo \
lzo2a_9x.lo lzo2a_d1.lo lzo2a_d2.lo alloc.lo io.lo lzo_crc.lo \
lzo_dll.lo lzo_init.lo lzo_ptr.lo lzo_str.lo lzo_util.lo
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --mode=compile $(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LINK = $(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP = --best
SOURCES = $(liblzo_la_SOURCES)
OBJECTS = $(liblzo_la_OBJECTS)

default: all

.SUFFIXES:
.SUFFIXES: .S .asm .c .i .lo .o .s .sh
$(srcdir)/Makefile.in: #M# Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu src/Makefile

Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-libLTLIBRARIES:

clean-libLTLIBRARIES:
	-test -z "$(lib_LTLIBRARIES)" || rm -f $(lib_LTLIBRARIES)

distclean-libLTLIBRARIES:

maintainer-clean-libLTLIBRARIES:

install-libLTLIBRARIES: $(lib_LTLIBRARIES)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(libdir)
	@list='$(lib_LTLIBRARIES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "$(LIBTOOL)  --mode=install $(INSTALL_DATA) $$p $(libdir)/$$p"; \
	    $(LIBTOOL)  --mode=install $(INSTALL_DATA) $$p $(libdir)/$$p; \
	  else :; fi; \
	done

uninstall-libLTLIBRARIES:
	@$(NORMAL_UNINSTALL)
	list='$(lib_LTLIBRARIES)'; for p in $$list; do \
	  $(LIBTOOL)  --mode=uninstall rm -f $(libdir)/$$p; \
	done

.c.o:
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

.S.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:

maintainer-clean-libtool:

liblzo.la: $(liblzo_la_OBJECTS) $(liblzo_la_DEPENDENCIES)
	$(LINK) -rpath $(libdir) $(liblzo_la_LDFLAGS) $(liblzo_la_OBJECTS) $(liblzo_la_LIBADD) $(LIBS)

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

subdir = src

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  test -f $(distdir)/$$file \
	  || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	  || cp -p $$d/$$file $(distdir)/$$file; \
	done
	$(MAKE) top_distdir="$(top_distdir)" distdir="$(distdir)" dist-hook
info:
dvi:
check: all
	$(MAKE)
installcheck:
install-exec: install-libLTLIBRARIES
	@$(NORMAL_INSTALL)

install-data: 
	@$(NORMAL_INSTALL)

install: install-exec install-data all
	@:

uninstall: uninstall-libLTLIBRARIES

all: Makefile $(LTLIBRARIES)

install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' INSTALL_SCRIPT='$(INSTALL_PROGRAM)' install
installdirs:
	$(mkinstalldirs)  $(libdir)


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
mostlyclean:  mostlyclean-libLTLIBRARIES mostlyclean-compile \
		mostlyclean-libtool mostlyclean-tags \
		mostlyclean-generic

clean:  clean-libLTLIBRARIES clean-compile clean-libtool clean-tags \
		clean-generic mostlyclean

distclean:  distclean-libLTLIBRARIES distclean-compile distclean-libtool \
		distclean-tags distclean-generic clean
	-rm -f config.status
	-rm -f libtool

maintainer-clean:  maintainer-clean-libLTLIBRARIES \
		maintainer-clean-compile maintainer-clean-libtool \
		maintainer-clean-tags maintainer-clean-generic \
		distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

.PHONY: default mostlyclean-libLTLIBRARIES distclean-libLTLIBRARIES \
clean-libLTLIBRARIES maintainer-clean-libLTLIBRARIES \
uninstall-libLTLIBRARIES install-libLTLIBRARIES mostlyclean-compile \
distclean-compile clean-compile maintainer-clean-compile \
mostlyclean-libtool distclean-libtool clean-libtool \
maintainer-clean-libtool tags mostlyclean-tags distclean-tags \
clean-tags maintainer-clean-tags distdir info dvi installcheck \
install-exec install-data install uninstall all installdirs \
mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


.s.o:
	$(COMPILE_S) -c $<

.s.lo:
	$(LIBTOOL) --mode=compile $(COMPILE_S) -c $<

.asm.o:
	$(COMPILE_ASM) -o $@ $<

.asm.lo:
	$(LIBTOOL) --mode=compile $(COMPILE_ASM) -o $@ $<

# /***********************************************************************
# // extra targets and dependencies
# ************************************************************************/

.c.i:
	$(CPP) $(DEFS) $(INCLUDES) $(CPPFLAGS) -o $@ $<

.s.i:
	$(CPP) $(ASFLAGS) $(DEFS) $(INCLUDES) $(CPPFLAGS) -o $@ $<

dist-hook:
	cp -rpi $(srcdir)/i386 $(distdir)/

#M##override o=.lo
#M##-include $(top_srcdir)/maint/src/src.mk

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
