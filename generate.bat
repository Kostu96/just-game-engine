@echo off

REM
REM Copyright (C) 2023 Konstanty Misiak
REM
REM SPDX-License-Identifier: MIT
REM

setlocal
pushd build

cmake .. ^
-D yaml-cpp_DIR="C:\Users\kmisiak\myplace\libs\yaml-cpp\lib\cmake\yaml-cpp" ^
-D msdf-atlas-gen_DIR="C:\Users\kmisiak\myplace\libs\msdf-atlas-gen\lib\cmake\msdf-atlas-gen" ^
-D msdfgen_DIR="C:\Users\kmisiak\myplace\libs\msdf-atlas-gen\lib\cmake\msdfgen"

popd
endlocal
