@echo off
if not exist build (
    mkdir build
)
pushd build

set CFLAGS=^
  /std:c++latest ^
  /EHsc ^
  /Od ^
  /Zi ^
  /nologo ^
  /FC ^
  /MTd

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