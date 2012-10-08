dnl aclocal.m4 generated automatically by aclocal 1.2f

dnl Copyright (C) 1994, 1995, 1996, 1997, 1998 Free Software Foundation, Inc.
dnl This Makefile.in is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

#
# Local additions to Autoconf macros.
#
#
# Copyright (C) 1998 Markus Franz Xaver Johannes Oberhumer
# Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer
# Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer
#



dnl mfx_CHECK_SIZEOF(TYPE [, CROSS-SIZE])
AC_DEFUN(mfx_CHECK_SIZEOF,
[changequote(<<, >>)dnl
dnl The name to #define.
define(<<AC_TYPE_NAME>>, translit(sizeof_$1, [a-z *], [A-Z_P]))dnl
dnl The cache variable name.
define(<<AC_CV_NAME>>, translit(ac_cv_sizeof_$1, [ *], [_p]))dnl
changequote([, ])dnl
AC_MSG_CHECKING([size of $1])
AC_CACHE_VAL(AC_CV_NAME,
[AC_TRY_RUN([#include <stdio.h>
#if STDC_HEADERS
#include <stdlib.h>
#include <stddef.h>
#endif
main() {
  FILE *f=fopen("conftestval", "w");
  if (!f) exit(1);
  fprintf(f, "%d\n", sizeof($1));
  exit(0);
}], AC_CV_NAME=`cat conftestval`, AC_CV_NAME=0, ifelse([$2], , , AC_CV_NAME=$2))])dnl
AC_MSG_RESULT($AC_CV_NAME)
AC_DEFINE_UNQUOTED(AC_TYPE_NAME, $AC_CV_NAME)
undefine([AC_TYPE_NAME])dnl
undefine([AC_CV_NAME])dnl
])



AC_DEFUN(mfx_PROG_GCC_PIPE,
[AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
if test "$ac_cv_prog_gcc" = yes; then
AC_CACHE_CHECK(whether ${CC-cc} accepts -pipe, mfx_cv_prog_gcc_pipe,
[echo 'main() { return 0; }' > conftest.c
if test -z "`${CC-cc} -pipe -c conftest.c 2>&1`"; then
  mfx_cv_prog_gcc_pipe=yes
else
  mfx_cv_prog_gcc_pipe=no
fi
rm -f conftest*
])
  if test "$mfx_cv_prog_gcc_pipe" = yes; then
    CC="$CC -pipe"
  fi
fi
])



AC_DEFUN(mfx_CHECK_HEADER_SANE_LIMITS_H,
[AC_CACHE_CHECK(whether limits.h is sane,
mfx_cv_header_sane_limits_h,
[AC_TRY_CPP([
#include <limits.h>
#if !defined(CHAR_BIT) || (CHAR_BIT != 8)
#  error CHAR_BIT
#endif
#if !defined(UCHAR_MAX)
#  error UCHAR_MAX
#endif
#if !defined(USHRT_MAX)
#  error USHRT_MAX
#endif
#if !defined(UINT_MAX)
#  error UINT_MAX
#endif
#if !defined(ULONG_MAX)
#  error ULONG_MAX
#endif
#if !defined(INT_MAX)
#  error INT_MAX
#endif
#if !defined(LONG_MAX)
#  error LONG_MAX
#endif
#if (UCHAR_MAX < 1)
#  error UCHAR_MAX
#endif
#if (USHRT_MAX < 1)
#  error USHRT_MAX
#endif
#if (UINT_MAX < 1)
#  error UINT_MAX
#endif
#if (ULONG_MAX < 1)
#  error ULONG_MAX
#endif
#if (UCHAR_MAX < 0xff)
#  error UCHAR_MAX
#endif
#if (USHRT_MAX < 0xffff)
#  error USHRT_MAX
#endif
#if (UINT_MAX < 0xffff)
#  error UINT_MAX
#endif
#if (ULONG_MAX < 0xffffffffL)
#  error ULONG_MAX
#endif
],
mfx_cv_header_sane_limits_h=yes,
mfx_cv_header_sane_limits_h=no)])
])



AC_DEFUN(mfx_PROG_GCC_BUG_STRENGTH_REDUCE,
[AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
if test "$ac_cv_prog_gcc" = yes; then
mfx_save_cflags="$CFLAGS"
CFLAGS="$CFLAGS -O2 -fstrength-reduce"
AC_CACHE_CHECK(whether ${CC-cc} suffers the -fstrength-reduce bug,
mfx_cv_prog_gcc_bug_strength_reduce,
[AC_TRY_RUN([
/* gcc strength-reduce optimization bug on Intel platforms.
 * Adapted from bug-report by John E. Davis <davis@space.mit.edu>
 * Compile and run it using gcc -O2 -fno-strength-reduce and
 * gcc -O2 -fstrength-reduce.
 */
int a[3];
unsigned an = 3;
int strength_reduce_bug();
main() {
  unsigned j;
  for (j = 0; j < an; j++)
    a[j] = (int)j - 3;
  exit(strength_reduce_bug() ? 1 : 0);
}
int strength_reduce_bug()
{ return a[0] != -3 || a[1] != -2 || a[2] != -1; }],
mfx_cv_prog_gcc_bug_strength_reduce=no,
mfx_cv_prog_gcc_bug_strength_reduce=yes,
mfx_cv_prog_gcc_bug_strength_reduce=unknown)])
CFLAGS="$mfx_save_cflags"
fi
])



AC_DEFUN(mfx_PROG_GCC_BUG_SCHEDULE_INSNS,
[AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
if test "$ac_cv_prog_gcc" = yes; then
mfx_save_cflags="$CFLAGS"
CFLAGS="$CFLAGS -O2 -fschedule-insns -fschedule-insns2"
AC_CACHE_CHECK(whether ${CC-cc} suffers the -fschedule-insns bug,
mfx_cv_prog_gcc_bug_schedule_insns,
[AC_TRY_RUN([main() {
/* gcc schedule-insns optimization bug on RS6000 platforms.
 * Adapted from bug-report by Assar Westerlund <assar@sics.se>
 * Compile and run it using gcc -O2 -fno-schedule-insns and
 * gcc -O2 -fschedule-insns.
 */
  const int clone[] = {1, 2, 0};
  const int *q;
  q = clone; if (*q) { return 0; }
  return 1; }],
mfx_cv_prog_gcc_bug_schedule_insns=no,
mfx_cv_prog_gcc_bug_schedule_insns=yes,
mfx_cv_prog_gcc_bug_schedule_insns=unknown)])
CFLAGS="$mfx_save_cflags"
fi
])



AC_DEFUN(mfx_PROG_CC_BUG_SIGNED_TO_UNSIGNED_CASTING,
[AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_CACHE_CHECK(whether signed to unsigned casting is broken,
mfx_cv_prog_cc_bug_signed_to_unsigned_casting,
[AC_TRY_RUN([main(){return((int)(unsigned char)((char) -1) != 255);}],
mfx_cv_prog_cc_bug_signed_to_unsigned_casting=no,
mfx_cv_prog_cc_bug_signed_to_unsigned_casting=yes,
mfx_cv_prog_cc_bug_signed_to_unsigned_casting=unknown)])
])



AC_DEFUN(mfx_PROG_CC_INTEGRAL_PROMOTION,
[AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_CACHE_CHECK(how the compiler promotes integrals,
mfx_cv_prog_cc_integral_promotion,
[AC_TRY_RUN([main(){ unsigned char c; int s;
  c = (unsigned char) (1 << (8 * sizeof(char) - 1));
  s = 8 * (int) (sizeof(int) - sizeof(char));
  exit(((c << s) > 0) ? 1 : 0);}],
mfx_cv_prog_cc_integral_promotion="ANSI (value-preserving)",
mfx_cv_prog_cc_integral_promotion="Classic (unsigned-preserving)",
mfx_cv_prog_cc_integral_promotion="unknown")])
])



AC_DEFUN(mfx_FUNC_MEMCMP,
[AC_CACHE_CHECK(for 8-bit clean memcmp, mfx_cv_func_memcmp,
[AC_TRY_RUN([main() {
  char c0 = 0x40, c1 = 0x80, c2 = 0x81;
  exit(memcmp(&c0, &c2, 1) < 0 && memcmp(&c1, &c2, 1) < 0 ? 0 : 1); }],
mfx_cv_func_memcmp=yes,
mfx_cv_func_memcmp=no,
mfx_cv_func_memcmp=unknown)])
if test "$mfx_cv_func_memcmp" = no; then
  AC_DEFINE(NO_MEMCMP)
fi
])



AC_DEFUN(mfx_C_BYTE_ORDER,
[AC_CACHE_CHECK(the byte order, mfx_cv_c_byte_order,
[mfx_cv_c_byte_order=unknown
AC_TRY_RUN([main() {
  /* Are we little or big endian?  From Harbison&Steele.  */
  union { long l; char c[sizeof(long)]; } u;
  u.l = 1; exit (u.c[sizeof(long) - 1] == 1);
}],
mfx_cv_c_byte_order=1234,
mfx_cv_c_byte_order=4321,
mfx_cv_c_byte_order=unknown)])
if test "$mfx_cv_c_byte_order" = 1234; then
  AC_DEFINE_UNQUOTED(MFX_BYTE_ORDER,1234)
fi
if test "$mfx_cv_c_byte_order" = 4321; then
  AC_DEFINE_UNQUOTED(MFX_BYTE_ORDER,4321)
fi
])



dnl mfx_PROG_MAKE_GNU(NAME)
AC_DEFUN(mfx_PROG_MAKE_GNU,
[AC_MSG_CHECKING([whether we are using GNU make])
# Run test in a subshell; some versions of sh will print an error if
# an executable is not found, even if stderr is redirected.
if (${MAKE-make} --version) 2>&1 </dev/null | grep 'GNU' >/dev/null; then
   $1=yes
   AC_MSG_RESULT(yes)
else
   $1=no
   AC_MSG_RESULT(no)
fi
AC_SUBST($1)])



AC_DEFUN(mfx_PROG_CC_CHECKERGCC,
[AC_CACHE_CHECK(whether we are using checkergcc, mfx_cv_prog_checkergcc,
[dnl The semicolon is to pacify NeXT's syntax-checking cpp.
cat > conftest.c <<EOF
#ifdef __GNUC__
#ifdef __CHECKER__
  yes;
#endif
#endif
EOF
if AC_TRY_COMMAND(${CC-cc} -E conftest.c) | egrep yes >/dev/null 2>&1; then
  mfx_cv_prog_checkergcc=yes
else
  mfx_cv_prog_checkergcc=no
fi])])



AC_DEFUN(mfx_CUSTOMIZE,
[
mfx_PROG_CC_BUG_SIGNED_TO_UNSIGNED_CASTING
mfx_PROG_GCC_BUG_SCHEDULE_INSNS
mfx_PROG_GCC_BUG_STRENGTH_REDUCE

# /***********************************************************************
# // Prepare some macros
# ************************************************************************/

CFLAGS_GCC_OMIT_FRAME_POINTER=""
if test "$GCC" = yes; then
  CFLAGS_GCC_OMIT_FRAME_POINTER="-fomit-frame-pointer"
  if test "$mfx_cv_prog_checkergcc" = yes; then
    CFLAGS_GCC_OMIT_FRAME_POINTER="-fno-omit-frame-pointer"
  fi
  if test "$enable_debug" = yes; then
    CFLAGS_GCC_OMIT_FRAME_POINTER="-fno-omit-frame-pointer"
  fi
  if test "$enable_profiling" = yes; then
    CFLAGS_GCC_OMIT_FRAME_POINTER="-fno-omit-frame-pointer"
  fi
  if test "$enable_coverage" = yes; then
    CFLAGS_GCC_OMIT_FRAME_POINTER="-fno-omit-frame-pointer"
  fi
fi
AC_SUBST(CFLAGS_GCC_OMIT_FRAME_POINTER)dnl

if test "$enable_debug" = yes; then
  if test "$ac_cv_prog_cc_g" = yes; then
    CFLAGS="$CFLAGS -g"
  fi
fi


# /***********************************************************************
# // Compiler and architecture specific stuff
# ************************************************************************/

AC_SUBST(M_CC)dnl
AC_SUBST(M_ARCH)dnl
AC_SUBST(M_CPU)dnl

M_CC="unknown"
M_ARCH="unknown"
M_CPU="$host_cpu"

if test "$GCC" = yes; then
  M_CC="GCC"
  CFLAGS_O="-O2"
  CFLAGS_W="-Wall -Wcast-align -Wwrite-strings"
  case $host in
    i[[34567]]86-*)
      M_ARCH="i386"
      mfx_unaligned_ok_2="yes"
      mfx_unaligned_ok_4="yes"
      CFLAGS_O="$CFLAGS_O -fno-strength-reduce"
      ;;
    *)
      if test "$mfx_cv_prog_gcc_bug_strength_reduce" = yes; then
        CFLAGS_O="$CFLAGS_O -fno-strength-reduce"
      fi
      ;;
  esac
  if test "$mfx_cv_prog_gcc_bug_schedule_insns" = yes; then
    CFLAGS_O="$CFLAGS_O -fno-schedule-insns -fno-schedule-insns2"
  fi
else
  CFLAGS_O="$CFLAGS_O"
# CFLAGS_O="$CFLAGS_O -O"
  CFLAGS_W="$CFLAGS_W"
fi

AC_DEFINE_UNQUOTED(MFX_ARCH,"$M_ARCH")
AC_DEFINE_UNQUOTED(MFX_CPU,"$host_cpu")

M_ARCH=`echo "$M_ARCH" | sed -e 's/[^a-zA-Z0-9]//g'`
M_CPU=`echo "$M_CPU" | sed -e 's/[^a-zA-Z0-9]//g'`
])


# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 1

dnl Usage:
dnl AM_INIT_AUTOMAKE(package,version, [no-define])

AC_DEFUN(AM_INIT_AUTOMAKE,
[AC_REQUIRE([AM_PROG_INSTALL])
PACKAGE=[$1]
AC_SUBST(PACKAGE)
VERSION=[$2]
AC_SUBST(VERSION)
dnl test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" && test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi
ifelse([$3],,
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE")
AC_DEFINE_UNQUOTED(VERSION, "$VERSION"))
AC_REQUIRE([AM_SANITY_CHECK])
AC_REQUIRE([AC_ARG_PROGRAM])
dnl FIXME This is truly gross.
missing_dir=`cd $ac_aux_dir && pwd`
AM_MISSING_PROG(ACLOCAL, aclocal, $missing_dir)
AM_MISSING_PROG(AUTOCONF, autoconf, $missing_dir)
AM_MISSING_PROG(AUTOMAKE, automake, $missing_dir)
AM_MISSING_PROG(AUTOHEADER, autoheader, $missing_dir)
AM_MISSING_PROG(MAKEINFO, makeinfo, $missing_dir)
AC_REQUIRE([AC_PROG_MAKE_SET])])


# serial 1

AC_DEFUN(AM_PROG_INSTALL,
[AC_REQUIRE([AC_PROG_INSTALL])
test -z "$INSTALL_SCRIPT" && INSTALL_SCRIPT='${INSTALL_PROGRAM}'
AC_SUBST(INSTALL_SCRIPT)dnl
])

#
# Check to make sure that the build environment is sane.
#

AC_DEFUN(AM_SANITY_CHECK,
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftestfile
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftestfile 2> /dev/null`
   if test "[$]*" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftestfile`
   fi
   if test "[$]*" != "X $srcdir/configure conftestfile" \
      && test "[$]*" != "X conftestfile $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "[$]2" = conftestfile
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
rm -f conftest*
AC_MSG_RESULT(yes)])

dnl AM_MISSING_PROG(NAME, PROGRAM, DIRECTORY)
dnl The program must properly implement --version.
AC_DEFUN(AM_MISSING_PROG,
[AC_MSG_CHECKING(for working $2)
# Run test in a subshell; some versions of sh will print an error if
# an executable is not found, even if stderr is redirected.
# Redirect stdin to placate older versions of autoconf.  Sigh.
if ($2 --version) < /dev/null > /dev/null 2>&1; then
   $1=$2
   AC_MSG_RESULT(found)
else
   $1="$3/missing $2"
   AC_MSG_RESULT(missing)
fi
AC_SUBST($1)])

# Like AC_CONFIG_HEADER, but automatically create stamp file.

AC_DEFUN(AM_CONFIG_HEADER,
[AC_PREREQ([2.12])
AC_CONFIG_HEADER([$1])
dnl When config.status generates a header, we must update the stamp-h file.
dnl This file resides in the same directory as the config header
dnl that is generated.  We must strip everything past the first ":",
dnl and everything past the last "/".
AC_OUTPUT_COMMANDS(changequote(<<,>>)dnl
ifelse(patsubst(<<$1>>, <<[^ ]>>, <<>>), <<>>,
<<test -z "<<$>>CONFIG_HEADERS" || echo timestamp > patsubst(<<$1>>, <<^\([^:]*/\)?.*>>, <<\1>>)stamp-h<<>>dnl>>,
<<am_indx=1
for am_file in <<$1>>; do
  case " <<$>>CONFIG_HEADERS " in
  *" <<$>>am_file "*<<)>>
    echo timestamp > `echo <<$>>am_file | sed -e 's%:.*%%' -e 's%[^/]*$%%'`stamp-h$am_indx
    ;;
  esac
  am_indx=`expr "<<$>>am_indx" + 1`
done<<>>dnl>>)
changequote([,]))])

# Add --enable-maintainer-mode option to configure.
# From Jim Meyering

# serial 1

AC_DEFUN(AM_MAINTAINER_MODE,
[AC_MSG_CHECKING([whether to enable maintainer-specific portions of Makefiles])
  dnl maintainer-mode is disabled by default
  AC_ARG_ENABLE(maintainer-mode,
[  --enable-maintainer-mode enable make rules and dependencies not useful
                          (and sometimes confusing) to the casual installer],
      USE_MAINTAINER_MODE=$enableval,
      USE_MAINTAINER_MODE=no)
  AC_MSG_RESULT($USE_MAINTAINER_MODE)
  if test $USE_MAINTAINER_MODE = yes; then
    MAINT=
  else
    MAINT='#M#'
  fi
  AC_SUBST(MAINT)dnl
]
)

# Define a conditional.

AC_DEFUN(AM_CONDITIONAL,
[AC_SUBST($1_TRUE)
AC_SUBST($1_FALSE)
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi])


# serial 24 AM_PROG_LIBTOOL
AC_DEFUN(AM_PROG_LIBTOOL,
[AC_REQUIRE([AM_ENABLE_SHARED])dnl
AC_REQUIRE([AM_ENABLE_STATIC])dnl
AC_REQUIRE([AC_CANONICAL_HOST])dnl
AC_REQUIRE([AC_PROG_RANLIB])dnl
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AM_PROG_LD])dnl
AC_REQUIRE([AM_PROG_NM])dnl
AC_REQUIRE([AC_PROG_LN_S])dnl
dnl
# Always use our own libtool.
LIBTOOL='$(SHELL) $(top_builddir)/libtool'
AC_SUBST(LIBTOOL)dnl

# Check for any special flags to pass to ltconfig.
libtool_flags=
test "$enable_shared" = no && libtool_flags="$libtool_flags --disable-shared"
test "$enable_static" = no && libtool_flags="$libtool_flags --disable-static"
test "$silent" = yes && libtool_flags="$libtool_flags --silent"
test "$ac_cv_prog_gcc" = yes && libtool_flags="$libtool_flags --with-gcc"
test "$ac_cv_prog_gnu_ld" = yes && libtool_flags="$libtool_flags --with-gnu-ld"

# Some flags need to be propagated to the compiler or linker for good
# libtool support.
case "$host" in
*-*-irix6*)
  # Find out which ABI we are using.
  echo '[#]line __oline__ "configure"' > conftest.$ac_ext
  if AC_TRY_EVAL(ac_compile); then
    case "`/usr/bin/file conftest.o`" in
    *32-bit*)
      LD="${LD-ld} -32"
      ;;
    *N32*)
      LD="${LD-ld} -n32"
      ;;
    *64-bit*)
      LD="${LD-ld} -64"
      ;;
    esac
  fi
  rm -rf conftest*
  ;;

*-*-sco3.2v5*)
  # On SCO OpenServer 5, we need -belf to get full-featured binaries.
  CFLAGS="$CFLAGS -belf"
  ;;
esac

# Actually configure libtool.  ac_aux_dir is where install-sh is found.
CC="$CC" CFLAGS="$CFLAGS" CPPFLAGS="$CPPFLAGS" \
LD="$LD" NM="$NM" RANLIB="$RANLIB" LN_S="$LN_S" \
${CONFIG_SHELL-/bin/sh} $ac_aux_dir/ltconfig \
$libtool_flags --no-verify $ac_aux_dir/ltmain.sh $host \
|| AC_MSG_ERROR([libtool configure failed])
])

# AM_ENABLE_SHARED - implement the --enable-shared flag
# Usage: AM_ENABLE_SHARED[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AM_ENABLE_SHARED,
[define([AM_ENABLE_SHARED_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(shared,
changequote(<<, >>)dnl
<<  --enable-shared         build shared libraries [default=>>AM_ENABLE_SHARED_DEFAULT]
changequote([, ])dnl
[  --enable-shared=PKGS    only build shared libraries if the current package
                          appears as an element in the PKGS list],
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_shared=yes ;;
no) enable_shared=no ;;
*)
  enable_shared=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_shared=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_shared=AM_ENABLE_SHARED_DEFAULT)dnl
])

# AM_DISABLE_SHARED - set the default shared flag to --disable-shared
AC_DEFUN(AM_DISABLE_SHARED,
[AM_ENABLE_SHARED(no)])

# AM_DISABLE_STATIC - set the default static flag to --disable-static
AC_DEFUN(AM_DISABLE_STATIC,
[AM_ENABLE_STATIC(no)])

# AM_ENABLE_STATIC - implement the --enable-static flag
# Usage: AM_ENABLE_STATIC[(DEFAULT)]
#   Where DEFAULT is either `yes' or `no'.  If omitted, it defaults to
#   `yes'.
AC_DEFUN(AM_ENABLE_STATIC,
[define([AM_ENABLE_STATIC_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(static,
changequote(<<, >>)dnl
<<  --enable-static         build static libraries [default=>>AM_ENABLE_STATIC_DEFAULT]
changequote([, ])dnl
[  --enable-static=PKGS    only build shared libraries if the current package
                          appears as an element in the PKGS list],
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_static=yes ;;
no) enable_static=no ;;
*)
  enable_static=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_static=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_static=AM_ENABLE_STATIC_DEFAULT)dnl
])


# AM_PROG_LD - find the path to the GNU or non-GNU linker
AC_DEFUN(AM_PROG_LD,
[AC_ARG_WITH(gnu-ld,
[  --with-gnu-ld           assume the C compiler uses GNU ld [default=no]],
test "$withval" = no || with_gnu_ld=yes, with_gnu_ld=no)
AC_REQUIRE([AC_PROG_CC])
ac_prog=ld
if test "$ac_cv_prog_gcc" = yes; then
  # Check if gcc -print-prog-name=ld gives a path.
  AC_MSG_CHECKING([for ld used by GCC])
  ac_prog=`($CC -print-prog-name=ld) 2>&5`
  case "$ac_prog" in
  # Accept absolute paths.
  /* | [A-Za-z]:\\*)
    test -z "$LD" && LD="$ac_prog"
    ;;
  "")
    # If it fails, then pretend we aren't using GCC.
    ac_prog=ld
    ;;
  *)
    # If it is relative, then search for the first ld in PATH.
    with_gnu_ld=unknown
    ;;
  esac
elif test "$with_gnu_ld" = yes; then
  AC_MSG_CHECKING([for GNU ld])
else
  AC_MSG_CHECKING([for non-GNU ld])
fi
AC_CACHE_VAL(ac_cv_path_LD,
[if test -z "$LD"; then
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  for ac_dir in $PATH; do
    test -z "$ac_dir" && ac_dir=.
    if test -f "$ac_dir/$ac_prog"; then
      ac_cv_path_LD="$ac_dir/$ac_prog"
      # Check to see if the program is GNU ld.  I'd rather use --version,
      # but apparently some GNU ld's only accept -v.
      # Break only if it was the GNU/non-GNU ld that we prefer.
      if "$ac_cv_path_LD" -v 2>&1 < /dev/null | egrep '(GNU|with BFD)' > /dev/null; then
	test "$with_gnu_ld" != no && break
      else
        test "$with_gnu_ld" != yes && break
      fi
    fi
  done
  IFS="$ac_save_ifs"
else
  ac_cv_path_LD="$LD" # Let the user override the test with a path.
fi])
LD="$ac_cv_path_LD"
if test -n "$LD"; then
  AC_MSG_RESULT($LD)
else
  AC_MSG_RESULT(no)
fi
test -z "$LD" && AC_MSG_ERROR([no acceptable ld found in \$PATH])
AC_SUBST(LD)
AM_PROG_LD_GNU
])

AC_DEFUN(AM_PROG_LD_GNU,
[AC_CACHE_CHECK([if the linker ($LD) is GNU ld], ac_cv_prog_gnu_ld,
[# I'd rather use --version here, but apparently some GNU ld's only accept -v.
if $LD -v 2>&1 </dev/null | egrep '(GNU|with BFD)' 1>&5; then
  ac_cv_prog_gnu_ld=yes
else
  ac_cv_prog_gnu_ld=no
fi])
])

# AM_PROG_NM - find the path to a BSD-compatible name lister
AC_DEFUN(AM_PROG_NM,
[AC_MSG_CHECKING([for BSD-compatible nm])
AC_CACHE_VAL(ac_cv_path_NM,
[case "$NM" in
/* | [A-Za-z]:\\*)
  ac_cv_path_NM="$NM" # Let the user override the test with a path.
  ;;
*)
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:"
  for ac_dir in /usr/ucb /usr/ccs/bin $PATH /bin; do
    test -z "$ac_dir" && ac_dir=.
    if test -f $ac_dir/nm; then
      # Check to see if the nm accepts a BSD-compat flag.
      # Adding the `sed 1q' prevents false positives on HP-UX, which says:
      #   nm: unknown option "B" ignored
      if ($ac_dir/nm -B /dev/null 2>&1 | sed '1q'; exit 0) | egrep /dev/null >/dev/null; then
        ac_cv_path_NM="$ac_dir/nm -B"
      elif ($ac_dir/nm -p /dev/null 2>&1 | sed '1q'; exit 0) | egrep /dev/null >/dev/null; then
        ac_cv_path_NM="$ac_dir/nm -p"
      else
        ac_cv_path_NM="$ac_dir/nm"
      fi
      break
    fi
  done
  IFS="$ac_save_ifs"
  test -z "$ac_cv_path_NM" && ac_cv_path_NM=nm
  ;;
esac])
NM="$ac_cv_path_NM"
AC_MSG_RESULT([$NM])
AC_SUBST(NM)
])


# serial 1

# @defmac AC_PROG_CC_STDC
# @maindex PROG_CC_STDC
# @ovindex CC
# If the C compiler in not in ANSI C mode by default, try to add an option
# to output variable @code{CC} to make it so.  This macro tries various
# options that select ANSI C on some system or another.  It considers the
# compiler to be in ANSI C mode if it handles function prototypes correctly.
#
# If you use this macro, you should check after calling it whether the C
# compiler has been set to accept ANSI C; if not, the shell variable
# @code{am_cv_prog_cc_stdc} is set to @samp{no}.  If you wrote your source
# code in ANSI C, you can make an un-ANSIfied copy of it by using the
# program @code{ansi2knr}, which comes with Ghostscript.
# @end defmac

AC_DEFUN(AM_PROG_CC_STDC,
[AC_REQUIRE([AC_PROG_CC])
AC_BEFORE([$0], [AC_C_INLINE])
AC_BEFORE([$0], [AC_C_CONST])
dnl Force this before AC_PROG_CPP.  Some cpp's, eg on HPUX, require
dnl a magic option to avoid problems with ANSI preprocessor commands
dnl like #elif.
dnl FIXME: can't do this because then AC_AIX won't work due to a
dnl circular dependency.
dnl AC_BEFORE([$0], [AC_PROG_CPP])
AC_MSG_CHECKING(for ${CC-cc} option to accept ANSI C)
AC_CACHE_VAL(am_cv_prog_cc_stdc,
[am_cv_prog_cc_stdc=no
ac_save_CC="$CC"
# Don't try gcc -ansi; that turns off useful extensions and
# breaks some systems' header files.
# AIX			-qlanglvl=ansi
# Ultrix and OSF/1	-std1
# HP-UX			-Aa -D_HPUX_SOURCE
# SVR4			-Xc -D__EXTENSIONS__
for ac_arg in "" -qlanglvl=ansi -std1 "-Aa -D_HPUX_SOURCE" "-Xc -D__EXTENSIONS__"
do
  CC="$ac_save_CC $ac_arg"
  AC_TRY_COMPILE(
[#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
/* Most of the following tests are stolen from RCS 5.7's src/conf.sh.  */
struct buf { int x; };
FILE * (*rcsopen) (struct buf *, struct stat *, int);
static char *e (p, i)
     char **p;
     int i;
{
  return p[i];
}
static char *f (char * (*g) (char **, int), char **p, ...)
{
  char *s;
  va_list v;
  va_start (v,p);
  s = g (p, va_arg (v,int));
  va_end (v);
  return s;
}
int test (int i, double x);
struct s1 {int (*f) (int a);};
struct s2 {int (*f) (double a);};
int pairnames (int, char **, FILE *(*)(struct buf *, struct stat *, int), int, int);
int argc;
char **argv;
], [
return f (e, argv, 0) != argv[0]  ||  f (e, argv, 1) != argv[1];
],
[am_cv_prog_cc_stdc="$ac_arg"; break])
done
CC="$ac_save_CC"
])
if test -z "$am_cv_prog_cc_stdc"; then
  AC_MSG_RESULT([none needed])
else
  AC_MSG_RESULT($am_cv_prog_cc_stdc)
fi
case "x$am_cv_prog_cc_stdc" in
  x|xno) ;;
  *) CC="$CC $am_cv_prog_cc_stdc" ;;
esac
])

# Check to see if we're running under Win32, without using
# AC_CANONICAL_*.  If so, set output variable EXEEXT to ".exe".
# Otherwise set it to "".

dnl AM_EXEEXT()
dnl This knows we add .exe if we're building in the Cygwin32
dnl environment. But if we're not, then it compiles a test program
dnl to see if there is a suffix for executables.
AC_DEFUN(AM_EXEEXT,
[AC_REQUIRE([AM_CYGWIN32])
AC_REQUIRE([AM_MINGW32])
AC_MSG_CHECKING([for executable suffix])
AC_CACHE_VAL(am_cv_exeext,
[if test "$CYGWIN32" = yes || test "$MINGW32" = yes; then
am_cv_exeext=.exe
else
cat > am_c_test.c << 'EOF'
int main() {
/* Nothing needed here */
}
EOF
${CC-cc} -o am_c_test $CFLAGS $CPPFLAGS $LDFLAGS am_c_test.c $LIBS 1>&5
am_cv_exeext=`echo am_c_test.* | grep -v am_c_test.c | sed -e s/am_c_test//`
rm -f am_c_test*])
test x"${am_cv_exeext}" = x && am_cv_exeext=no
fi
EXEEXT=""
test x"${am_cv_exeext}" != xno && EXEEXT=${am_cv_exeext}
AC_MSG_RESULT(${am_cv_exeext})
AC_SUBST(EXEEXT)])

# Check to see if we're running under Cygwin32, without using
# AC_CANONICAL_*.  If so, set output variable CYGWIN32 to "yes".
# Otherwise set it to "no".

dnl AM_CYGWIN32()
AC_DEFUN(AM_CYGWIN32,
[AC_CACHE_CHECK(for Cygwin32 environment, am_cv_cygwin32,
[AC_TRY_COMPILE(,[return __CYGWIN32__;],
am_cv_cygwin32=yes, am_cv_cygwin32=no)
rm -f conftest*])
CYGWIN32=
test "$am_cv_cygwin32" = yes && CYGWIN32=yes])

# Check to see if we're running under Mingw, without using
# AC_CANONICAL_*.  If so, set output variable MINGW32 to "yes".
# Otherwise set it to "no".

dnl AM_MINGW32()
AC_DEFUN(AM_MINGW32,
[AC_CACHE_CHECK(for Mingw32 environment, am_cv_mingw32,
[AC_TRY_COMPILE(,[return __MINGW32__;],
am_cv_mingw32=yes, am_cv_mingw32=no)
rm -f conftest*])
MINGW32=
test "$am_cv_mingw32" = yes && MINGW32=yes])

