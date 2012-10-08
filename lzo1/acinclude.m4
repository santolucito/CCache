#
# Local additions to Autoconf macros.
#
#
# Copyright (C) 1998 Markus Franz Xaver Johannes Oberhumer
# Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer
# Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer
#


## --------------------------------------------------------- ##
## Includes <stdlib.h> and <stddef.h> if available.          ##
## Adapted from AC_CHECK_SIZEOF (Autoconf 2.12).             ##
## --------------------------------------------------------- ##

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


## --------------------------------------------------------- ##
## Check if gcc accepts '-pipe'.                             ##
## Adapted from AC_PROG_CC and AC_PROG_GCC_TRADITIONAL.      ##
## --------------------------------------------------------- ##

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


## --------------------------------------------------------- ##
## Check for a sane <limits.h> header.                       ##
## --------------------------------------------------------- ##

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


## --------------------------------------------------------- ##
## Check if gcc suffers the '-fstrength-reduce' bug.         ##
## --------------------------------------------------------- ##

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


## --------------------------------------------------------- ##
## Check if gcc suffers the '-fschedule-insns' bug.          ##
## --------------------------------------------------------- ##

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


## --------------------------------------------------------- ##
## Checking if compiler correctly cast signed to unsigned.   ##
## Adapted from zsh.                                         ##
## --------------------------------------------------------- ##

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


## --------------------------------------------------------- ##
## Check how the compiler promotes integrals.                ##
## --------------------------------------------------------- ##

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


## --------------------------------------------------------- ##
## Check for 8-bit clean memcmp.                             ##
## Adapted from AC_FUNC_MEMCMP.                              ##
## --------------------------------------------------------- ##

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


## --------------------------------------------------------- ##
## Check the byte order.                                     ##
## Adapted from AC_C_BIGENDIAN.                              ##
## --------------------------------------------------------- ##

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


## --------------------------------------------------------- ##
## Check for GNU make (do not cache this setting)            ##
## Adapted from AM_MISSING_PROG and AM_PROG_LD_GNU.          ##
## --------------------------------------------------------- ##

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


## --------------------------------------------------------- ##
## Check for checkergcc.                                     ##
## Adapted from AC_PROG_CC_GNU.                              ##
## --------------------------------------------------------- ##

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


## --------------------------------------------------------- ##
## My own customization.                                     ##
## --------------------------------------------------------- ##

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

