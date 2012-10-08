#
# Common stuff for all Makefiles - requires GNU make
#
# Copyright (C) 1996, 1997, 1998 Markus F.X.J. Oberhumer
#

.SUFFIXES: .a .asm .c .dll .exe .i .la .lib .lo .o .obj .out .s .S .sh

srcdir = .
top_srcdir = .
top_builddir = .


MFX_CC := $(strip $(firstword $(CC)))

ifeq ($(strip $(MFX_USE_GCC)),)
ifeq ($(MFX_CC),gcc)
MFX_USE_GCC = yes
endif
ifeq ($(MFX_CC),gxx)
MFX_USE_GCC = yes
endif
ifeq ($(MFX_CC),g++)
MFX_USE_GCC = yes
endif
ifeq ($(MFX_CC),egcs)
MFX_USE_GCC = yes
endif
ifeq ($(MFX_CC),pgcc)	# Pentium gcc
MFX_USE_GCC = yes
endif
endif


@bs = $(subst /,\\,$@)
<bs = $(subst /,\\,$<)
^bs = $(subst /,\\,$^)

CFLAGS += -DNO_ZLIB_H
ifeq ($(strip $(COMPILE_C)),)
ifeq ($(strip $(COMSPEC)),)
COMPILE_C = $(CC) $(CFLAGS) -c $<
else
COMPILE_C = $(CC) $(CFLAGS) -c $(<bs)
endif
endif


ifeq ($(b_dos16)$(b_win16),)
extra_test_PROGRAMS += dict$(exe) lpack$(exe) overlap$(exe)
extra_test_PROGRAMS += precomp$(exe) precomp2$(exe)
endif


# /***********************************************************************
# //
# ************************************************************************/

liblzo_lib_SOURCES = \
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

LZO_ASM_SOURCES_GCC_i386 = \
    lzo1c_s1.s lzo1c_s2.s \
    lzo1f_f1.s lzo1f_f2.s \
    lzo1x_f1.s lzo1x_f2.s lzo1x_s1.s lzo1x_s2.s \
    lzo1y_f1.s lzo1y_f2.s lzo1y_s1.s lzo1y_s2.s

LZO_ASM_SOURCES_WC_i386 = $(subst .s,.asm,$(LZO_ASM_SOURCES_GCC_i386))

liblzo_SOURCES += $(liblzo_lib_SOURCES) $(liblzo_extra_SOURCES)

test_PROGRAMS += chksum$(exe) promote$(exe) sizes$(exe) simple$(exe)
test_PROGRAMS += $(extra_test_PROGRAMS)

-include maint/b/lzo.mk

liblzo_c_OBJECTS   := $(subst .c,$o,$(filter %.c,$(liblzo_SOURCES)))
liblzo_s_OBJECTS   := $(subst .s,$o,$(filter %.s,$(liblzo_SOURCES)))
liblzo_asm_OBJECTS := $(subst .asm,$o,$(filter %.asm,$(liblzo_SOURCES)))

liblzo_OBJECTS := $(liblzo_c_OBJECTS) $(liblzo_s_OBJECTS) $(liblzo_asm_OBJECTS)
ifneq ($(objdir),)
liblzo_OBJECTS := $(addprefix $(objdir),$(liblzo_OBJECTS))
endif


# /***********************************************************************
# // main targets
# ************************************************************************/

.PHONY: default all lib bin clean install

default all: lib bin

lib: $(liblzo)

$(liblzo): $(liblzo_OBJECTS)
	rm -f $@
	$(LZO_ARLIB)

bin: ltest$(exe) $(test_PROGRAMS)

ltest$(exe) : %$(exe) : %$(o) $(liblzo) $(ltest_LDADD)
	$(LZO_LINK) $(ltest_LIBS)

$(filter-out ltest$(exe),$(test_PROGRAMS)): %$(exe) : %$o $(liblzo)
	$(LZO_LINK)

%$o : %.c
	$(COMPILE_C)

%$o : %.s
	$(LZO_COMPILE_S)

$(liblzo_c_OBJECTS): %$o : %.c
	$(LZO_COMPILE_C)

$(liblzo_s_OBJECTS): %$o : %.s
	$(LZO_COMPILE_S)

$(liblzo_asm_OBJECTS): %$o : %.asm
	$(LZO_COMPILE_ASM)

clean:
	rm -f *.o *.obj *.err core $(liblzo) $(test_PROGRAMS)


# /***********************************************************************
# // extra dependencies
# ************************************************************************/

ltest$o: $(top_srcdir)/include/lzoconf.h $(srcdir)/timer.h
lzo_util$o: $(top_srcdir)/include/lzoconf.h

-include maint/src/src.mk

