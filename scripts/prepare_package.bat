@echo off

REM
REM Copyright (C) 2022 Konstanty Misiak
REM
REM SPDX-License-Identifier: MIT
REM

setlocal
pushd build

set PACKAGE_DIR=.\package\
set OUTPUT_DIR=.\output\
set THIRD_PARTY_DIR=..\third_party\

cmake ..
cmake --build . --target jng --config Debug
cmake --build . --target jng --config Release

robocopy ..\include %PACKAGE_DIR%include /E /NJH /NJS
robocopy %THIRD_PARTY_DIR%entt\include\entt %PACKAGE_DIR%include\entt /E /NJH /NJS
robocopy %THIRD_PARTY_DIR%entt %PACKAGE_DIR%include\entt LICENSE /NJH /NJS
robocopy %THIRD_PARTY_DIR%glm\glm %PACKAGE_DIR%include\glm *.hpp *.inl /E /NJH /NJS
robocopy %THIRD_PARTY_DIR%glm %PACKAGE_DIR%include\glm copying.txt /NJH /NJS
robocopy %THIRD_PARTY_DIR%imgui %PACKAGE_DIR%include\imgui imgui.h LICENSE.txt /NJH /NJS
robocopy %THIRD_PARTY_DIR%spdlog\include\spdlog %PACKAGE_DIR%include\spdlog /E /NJH /NJS
robocopy %THIRD_PARTY_DIR%spdlog %PACKAGE_DIR%include\spdlog LICENSE /NJH /NJS

robocopy %OUTPUT_DIR%debug\lib %PACKAGE_DIR%lib\debug /E /NJH /NJS
robocopy %VULKAN_SDK%\Lib %PACKAGE_DIR%lib\debug vulkan-1.lib shaderc_combinedd.lib spirv-cross-cored.lib spirv-cross-glsld.lib spirv-cross-hlsld.lib /NJH /NJS

robocopy %OUTPUT_DIR%release\lib %PACKAGE_DIR%lib\release /E /NJH /NJS
robocopy %VULKAN_SDK%\Lib %PACKAGE_DIR%lib\release vulkan-1.lib shaderc_combined.lib spirv-cross-core.lib spirv-cross-glsl.lib spirv-cross-hlsl.lib /NJH /NJS

robocopy ..\assets %PACKAGE_DIR%bin\assets /E /NJH /NJS

robocopy .. %PACKAGE_DIR% LICENSE /NJH /NJS

popd
endlocal
