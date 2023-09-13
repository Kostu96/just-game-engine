@echo off

REM
REM Copyright (C) 2023 Konstanty Misiak
REM
REM SPDX-License-Identifier: MIT
REM

setlocal

set FREETYPE_DIR=%cd%\dependencies\freetype\build\install

cmake -P scripts/cmake/prepare_dependencies.cmake

cmake -S . -B build ^
    -Dbox2d_DIR="dependencies/box2d/build/install/lib/cmake/box2d" ^
    -Dglfw3_DIR="dependencies/glfw3/build/install/lib/cmake/glfw3" ^
    -Dglm_DIR="dependencies/glm/build/install/lib/cmake/glm" ^
    -DLua_DIR="dependencies/Lua/build/install/lib/cmake/lua" ^
    -Dmsdf-atlas-gen_DIR="dependencies/msdf-atlas-gen/build/install/lib/cmake/msdf-atlas-gen" ^
    -Dmsdfgen_DIR="dependencies/msdf-atlas-gen/build/install/lib/cmake/msdfgen" ^
    -Dspdlog_DIR="dependencies/spdlog/build/install/lib/cmake/spdlog" ^
    -Dyaml-cpp_DIR="dependencies/yaml-cpp/build/install/lib/cmake/yaml-cpp" ^
    -Dglad_DIR="dependencies/glad/build/install/lib/cmake/glad" ^
    -Dstb_DIR="dependencies/stb/build/install/lib/cmake/stb"

endlocal
