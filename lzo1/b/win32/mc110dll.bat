rem /* Windows 32 bit (DLL) - Microsoft 32-bit C/C++ Compiler 11.00
rem  * a very simple make driver
rem  * Copyright (C) 1996, 1997, 1998 Markus F.X.J. Oberhumer
rem  */

@if "%LZO_ECHO%"=="n" echo off

set _CC=cl -nologo -MD
set CFLAGS=-Iinclude -O2 -GF -W3
set MYDLL=lzo.dll
set MYLIB=lzo.lib

echo Compiling, please be patient...
set CC=%_CC% -D__LZO_EXPORT1#__declspec(dllexport)
%CC% %CFLAGS% -c src\*.c
@if errorlevel 1 goto error
if exist %MYDLL% del %MYDLL%
if exist %MYLIB% del %MYLIB%
link @b\win32\mc110dll.def -implib:%MYLIB% -out:%MYDLL% @b\win32\mc110dll.rsp
@if errorlevel 1 goto error

set CC=%_CC% -DLZO_NO_ASM 
REM set CC=%CC% -D__LZO_EXPORT1#__declspec(dllimport)
%CC% %CFLAGS% ltest\ltest.c %MYLIB% setargv.obj
@if errorlevel 1 goto error

%CC% %CFLAGS% examples\dict.c %MYLIB% setargv.obj
@if errorlevel 1 goto error
%CC% %CFLAGS% examples\lpack.c %MYLIB% setargv.obj
@if errorlevel 1 goto error
%CC% %CFLAGS% examples\overlap.c %MYLIB% setargv.obj
@if errorlevel 1 goto error
%CC% %CFLAGS% examples\precomp.c %MYLIB% setargv.obj
@if errorlevel 1 goto error
%CC% %CFLAGS% examples\precomp2.c %MYLIB% setargv.obj
@if errorlevel 1 goto error
%CC% %CFLAGS% examples\simple.c %MYLIB% setargv.obj
@if errorlevel 1 goto error

%CC% %CFLAGS% -Isrc tests\align.c %MYLIB%
@if errorlevel 1 goto error
%CC% %CFLAGS% -Isrc tests\chksum.c %MYLIB%
@if errorlevel 1 goto error

set CC=%_CC%
%CC% %CFLAGS% -Fetestmini.exe minilzo\*.c
@if errorlevel 1 goto error

echo Done.
goto end
:error
echo error!
:end
@call b\unset.bat
