Simple make drivers for DOS, Windows, OS/2 and other systems
============================================================

This directory contains a bunch of simple make drivers - I've tried
to make them as foolproof as possible.

To build LZO for your system type `b\OS\COMPILER' in the base directory,
e.g. `b\dos32\wc105' will build the 32-bit DOS Watcom C v10.5 version.

After building do a basic test by running `ltest -mlzo COPYING'.
util\check.sh is an example of a more thorough test driver.

Please send me your additional/improved versions.


Overview:
---------

b\dos16\bc40.bat        Borland C 4.0
b\dos16\mc70.bat        Microsoft C 7.0
b\dos16\wc105.bat       Watcom C 10.5

b\dos32\dj1.bat         djgpp v1 + gcc 2.6.3                  (1)
b\dos32\dj2.bat         djgpp v2 + gcc 2.7.2.1                (1)
b\dos32\dj2_pgcc.bat    djgpp v2 + Pentium gcc 980102         (1)
b\dos32\emx.bat         emx + gcc 2.7.2.1                     (1)
b\dos32\wc105.bat       Watcom C 10.5                         (1)

b\os2\emx_aout.cmd      emx A.OUT + gcc 2.7.2.1               (1) (3)
b\os2\wc105.cmd         Watcom C 10.5                         (1) (3)

b\palmos\gcc_lib.sh     gcc 2.7.2 cross-compiler

b\tos\gcc258.g          gcc 2.5.8
b\tos\pc.prj            Pure C 1.0 (Sep 20 1991)

b\win16\wc105.bat       Watcom C 10.5
b\win16\wc105dll.bat    Watcom C 10.5 DLL

b\win32\cdk.bat         Cygnus Win32 + gcc                    (1)
b\win32\mc110.bat       Microsoft C 11.00      Visual C 5.0   (2)
b\win32\mc110dll.bat    Microsoft C 11.00 DLL
b\win32\wc105.bat       Watcom C 10.5                         (1)
b\win32\wc105dll.bat    Watcom C 10.5 DLL


Notes:
  (1) includes support for i386 assembler versions
  (2) you need MASM (or another assembler) for the assembler versions
  (3) not tested as of version 1.04



Makefiles for DOS, Windows, OS/2 and other systems
==================================================

I've also added "real" Makefiles for many platforms. You need some
freely available tools like GNU `make' and GNU `rm'.

To use them type `make -f b\OS\COMPILER.mak' in the base directory,
e.g. `gnumake -f b\dos32\wc105.mak'.

The Makefiles are equivalent to the build scripts - so don't worry
if you don't have the required utilities or have other problems.


Overview:
---------
b\dos16\mc70.mak
b\dos16\wc105.mak
b\dos32\dj2.mak
b\dos32\dj2_pgcc.mak
b\dos32\emx.mak
b\dos32\wc105.mak
b\os2\wc105.mak
b\win16\wc105.mak
b\win32\cdk.mak
b\win32\mc110.mak
b\win32\wc105.mak



Useful tools
============

GNU make, GNU fileutils for DOS / Windows 3.1 / Windows 95:
   ftp://ftp.simtel.net/pub/simtelnet/gnu/djgpp/v2gnu/

GNU make, GNU fileutils for Windows NT
   ftp://ftp.cygnus.com

GNU make, GNU fileutils for OS2
   ftp://hobbes.nmsu.edu/pub/os2/

GNU make, GNU fileutils, gulam, MiNT for TOS (Atari ST)
   ftp://archive.umich.edu

Palm Pilot Software Development
   http://www.massena.com/darrin/pilot/index.html

