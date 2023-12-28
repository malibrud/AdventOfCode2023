@echo off

if not exist build (
    mkdir build
)

if "%1" EQU "clean" (
    del build\*.*
    exit /b
)
pushd build

if "%1" EQU "debug" (
    set OPTFLAGS=/Od 
) else if "%1" EQU "release" (
    set OPTFLAGS=/O2 
) else (
    set OPTFLAGS=/Od 
)

set CFLAGS=^
  /std:c++latest ^
  /EHsc ^
  %OPTFLAGS% ^
  /MDd ^
  /Zi ^
  /nologo ^
  /FC 

if not exist std.obj (
    cl %CFLAGS% /c "%VCToolsInstallDir%\modules\std.ixx"
)
if not exist std.compat.obj (
    cl %CFLAGS% /c "%VCToolsInstallDir%\modules\std.compat.ixx"
)

set OBJS=std.obj std.compat.obj

cl %CFLAGS% ..\part1.cpp %OBJS%
cl %CFLAGS% ..\part2.cpp %OBJS%

popd