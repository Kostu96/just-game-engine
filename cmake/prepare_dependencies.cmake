#
# Copyright (C) 2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

find_package(Git QUIET REQUIRED)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies OUTPUT_QUIET)

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    add_library(vulkan-shaderc SHARED IMPORTED GLOBAL)
    add_library(vulkan-spirv-cross-core STATIC IMPORTED GLOBAL)
    add_library(vulkan-spirv-cross-glsl STATIC IMPORTED GLOBAL)
    if (WIN32)
        set_target_properties(vulkan-shaderc PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/Bin/shaderc_shared.dll
            IMPORTED_IMPLIB $ENV{VULKAN_SDK}/Lib/shaderc_shared.lib
            IMPORTED_LOCATION_DEBUG $ENV{VULKAN_SDK}/Bin/shaderc_sharedd.dll
            IMPORTED_IMPLIB_DEBUG $ENV{VULKAN_SDK}/Lib/shaderc_sharedd.lib
            IMPORTED_CONFIGURATIONS "Release;Debug")
        set_target_properties(vulkan-spirv-cross-core PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/spirv-cross-core.lib
            IMPORTED_LOCATION_DEBUG $ENV{VULKAN_SDK}/Lib/spirv-cross-cored.lib
            IMPORTED_CONFIGURATIONS "Release;Debug")
        set_target_properties(vulkan-spirv-cross-glsl PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/spirv-cross-glsl.lib
            IMPORTED_LOCATION_DEBUG $ENV{VULKAN_SDK}/Lib/spirv-cross-glsld.lib
            IMPORTED_CONFIGURATIONS "Release;Debug")
    else()
        set_target_properties(vulkan-shaderc PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/lib/libshaderc_combined.a
            IMPORTED_CONFIGURATIONS "Release")
        set_target_properties(vulkan-spirv-cross-core PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/lib/libspirv-cross-core.a
            IMPORTED_CONFIGURATIONS "Release")
        set_target_properties(vulkan-spirv-cross-glsl PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/lib/libspirv-cross-glsl.a
            IMPORTED_CONFIGURATIONS "Release")
    endif()
        
    add_library(VulkanShaderTools INTERFACE)
    target_include_directories(VulkanShaderTools INTERFACE ${Vulkan_INCLUDE_DIR})
    target_link_libraries(VulkanShaderTools INTERFACE vulkan-shaderc vulkan-spirv-cross-core vulkan-spirv-cross-glsl)
    target_link_options(VulkanShaderTools INTERFACE /ignore:4099) # NOTE: ignore warning about pdb files for debug libraries.
endif()

message(STATUS "Preparing msdf-atlas-gen...")
if (NOT EXISTS "${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/Kostu96/msdf-atlas-gen-fork.git --depth 1 --shallow-submodules --recurse-submodules WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies OUTPUT_QUIET)
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork -B ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build -DMSDF_ATLAS_INSTALL=ON -DMSDF_ATLAS_DYNAMIC_RUNTIME=ON OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build --config Debug OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build --config Release OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/install OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build --config Debug --prefix "${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/install" OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build --config Release --prefix "${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/install" OUTPUT_QUIET)
set(msdf-atlas-gen_DIR ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/install/lib/cmake/msdf-atlas-gen)
set(msdfgen_DIR ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/install/lib/cmake/msdfgen)
find_package(msdf-atlas-gen REQUIRED)

message(STATUS "Preparing yaml-cpp...")
if (NOT EXISTS "${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/jbeder/yaml-cpp.git --depth 1 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies OUTPUT_QUIET)
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp -B ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build --config Debug OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build --config Release OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/install OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build --config Debug --prefix "${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/install" OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build --config Release --prefix "${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/install" OUTPUT_QUIET)
set(yaml-cpp_DIR ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/install/lib/cmake/yaml-cpp)
find_package(yaml-cpp REQUIRED)