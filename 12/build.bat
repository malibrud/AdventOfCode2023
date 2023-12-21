@echo off

if not exist build (
    mkdir build
)
pushd build

if "%1" EQU "debug" (
    set OPTFLAGS=/Od /MTd
) else if "%1" EQU "release" (
    set OPTFLAGS=/O2 /MT
) else (
    set OPTFLAGS=/Od /MTd
)

set CFLAGS=^
  /std:c++latest ^
  /EHsc ^
  %OPTFLAGS% ^
  /Zi ^
  /nologo ^
  /FC 

if not exist std.obj (
    cl %CFLAGS% "%VCToolsInstallDir%\modules\std.ixx"
)
if not exist std.compat.obj (
    cl %CFLAGS% "%VCToolsInstallDir%\modules\std.compat.ixx"
)

set OBJS=std.obj std.compat.obj

cl %CFLAGS% ..\part1.cpp %OBJS%
cl %CFLAGS% ..\part2.cpp %OBJS%

popd