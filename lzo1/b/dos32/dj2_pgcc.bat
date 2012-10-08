rem /* DOS 32 bit - pgcc-2.90.23 980102 (egcs-1.0.1 release)
rem  * a very simple make driver
rem  * Copyright (C) 1996, 1997, 1998 Markus F.X.J. Oberhumer
rem  */

@if "%LZO_ECHO%"=="n" echo off

set PGCC_CFLAGS=@b/dos32/dj2_pgcc.opt
set EXTRA_CFLAGS=

set CC=pgcc
set CFLAGS=@b/gcc.opt @b/dos32/dj2.opt @b/gcc_lzo.opt %EXTRA_CFLAGS%
set ASFLAGS=-x assembler-with-cpp -Wall
set MYLIB=liblzo.a

echo Compiling, please be patient...
%CC% %CFLAGS% %PGCC_CFLAGS% -c src/*.c
@if errorlevel 1 goto error
%CC% %ASFLAGS% -c src/i386/src/*.s
@if errorlevel 1 goto error
if exist %MYLIB% del %MYLIB%
ar rcs %MYLIB% @b/dos32/dj2.rsp
@if errorlevel 1 goto error

set CFLAGS=@b/gcc.opt @b/dos32/dj2.opt %EXTRA_CFLAGS%
%CC% -s %CFLAGS% ltest/ltest.c %MYLIB% -o ltest.exe
@if errorlevel 1 goto error

%CC% -s %CFLAGS% examples/dict.c %MYLIB% -o dict.exe
@if errorlevel 1 goto error
%CC% -s %CFLAGS% examples/lpack.c %MYLIB% -o lpack.exe
@if errorlevel 1 goto error
%CC% -s %CFLAGS% examples/overlap.c %MYLIB% -o overlap.exe
@if errorlevel 1 goto error
%CC% -s %CFLAGS% examples/precomp.c %MYLIB% -o precomp.exe
@if errorlevel 1 goto error
%CC% -s %CFLAGS% examples/precomp2.c %MYLIB% -o precomp2.exe
@if errorlevel 1 goto error
%CC% -s %CFLAGS% examples/simple.c %MYLIB% -o simple.exe
@if errorlevel 1 goto error

%CC% -s %CFLAGS% -Isrc tests/align.c %MYLIB% -o align.exe
@if errorlevel 1 goto error
%CC% -s %CFLAGS% -Isrc tests/chksum.c %MYLIB% -o chksum.exe
@if errorlevel 1 goto error

set CFLAGS=@b/gcc.opt @b/dos32/dj2.opt @b/gcc_lzo.opt %EXTRA_CFLAGS%
@REM workaround a bug in the pgcc driver
%CC% %CFLAGS% %PGCC_CFLAGS% -c minilzo/*.c
%CC% -s %CFLAGS% minilzo.o testmini.o -o testmini.exe
@if errorlevel 1 goto error

echo Done.
goto end
:error
echo error!
:end
@call b\unset.bat
