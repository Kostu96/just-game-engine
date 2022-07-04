#
# Copyright (C) 2020-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

cmake_minimum_required(VERSION 3.20)

### options: ###
option(JNG_BUILD_SAMPLES "Build sample projects" ON)
################

project(jng-proj LANGUAGES CXX)
include(cmake/base_configuration.cmake)
include(cmake/compiler_warnings.cmake)

set(JNG_TARGET_NAME jng)

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    #if (WIN32)
    #    list(APPEND Vulkan_LIBRARIES
    #        $ENV{VULKAN_SDK}/Lib/shaderc_combined$<$<CONFIG:Debug>:d>.lib
    #        $ENV{VULKAN_SDK}/Lib/spirv-cross-core$<$<CONFIG:Debug>:d>.lib
    #        $ENV{VULKAN_SDK}/Lib/spirv-cross-glsl$<$<CONFIG:Debug>:d>.lib
    #    )
    #else()
    #    list(APPEND Vulkan_LIBRARIES
    #        $ENV{VULKAN_SDK}/lib/shaderc_combined.a
    #        $ENV{VULKAN_SDK}/lib/spirv-cross-core.a
    #        $ENV{VULKAN_SDK}/lib/spirv-cross-glsl.a
    #    )
    #endif()
    add_library(vulkan-shaderc STATIC IMPORTED)
    add_library(vulkan-shadercd STATIC IMPORTED)
    add_library(vulkan-spirv-cross-core STATIC IMPORTED)
    add_library(vulkan-spirv-cross-cored STATIC IMPORTED)
    add_library(vulkan-spirv-cross-glsl STATIC IMPORTED)
    add_library(vulkan-spirv-cross-glsld STATIC IMPORTED)
    if (WIN32)
        set_target_properties(vulkan-shaderc PROPERTIES IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/shaderc_combined.lib)
        set_target_properties(vulkan-shadercd PROPERTIES IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/shaderc_combinedd.lib)
        set_target_properties(vulkan-spirv-cross-core PROPERTIES IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/spirv-cross-core.lib)
        set_target_properties(vulkan-spirv-cross-cored PROPERTIES IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/spirv-cross-cored.lib)
        set_target_properties(vulkan-spirv-cross-glsl PROPERTIES IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/spirv-cross-glsl.lib)
        set_target_properties(vulkan-spirv-cross-glsld PROPERTIES IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/spirv-cross-glsld.lib)
    else()
        set_target_properties(vulkan-shaderc PROPERTIES IMPORTED_LOCATION $ENV{VULKAN_SDK}/lib/shaderc_combined.a)
    endif()
    
    list(APPEND Vulkan_LIBRARIES
        vulkan-shaderc$<$<CONFIG:Debug>:d>
        vulkan-spirv-cross-core$<$<CONFIG:Debug>:d>
        vulkan-spirv-cross-glsl$<$<CONFIG:Debug>:d>
    )
endif()

add_subdirectory(third_party)

set(JNG_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/jng)
add_subdirectory(source)

add_library(${JNG_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${JNG_HEADERS}
    ${JNG_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/jng PREFIX source FILES ${JNG_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${JNG_SOURCES})

target_compile_definitions(${JNG_TARGET_NAME} PUBLIC
    $<$<CONFIG:Debug>:JNG_DEBUG>
    IMGUI_USER_CONFIG="jng/imconfig.hpp"
)
if (WIN32)
    target_compile_definitions(${JNG_TARGET_NAME} PUBLIC
        JNG_WINDOWS
    )
endif()

set_target_warnings(${JNG_TARGET_NAME})

target_compile_options(${JNG_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

set_target_properties(${JNG_TARGET_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

target_include_directories(${JNG_TARGET_NAME} SYSTEM
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm
    PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    #${CMAKE_CURRENT_SOURCE_DIR}/third_party/entt/include
)

target_include_directories(${JNG_TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/jng
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_link_options(${JNG_TARGET_NAME} PUBLIC
    /ignore:4099
)

target_link_libraries(${JNG_TARGET_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
    cpp-common-lib
    glad
    glfw
    imgui
    spdlog
    stb
)

if (WIN32)
    target_link_libraries(${JNG_TARGET_NAME} PUBLIC
        $<$<CONFIG:Debug>:dxerr>
    )
endif()

if (JNG_BUILD_SAMPLES)
    message(STATUS "Building sample projects")
    add_subdirectory(samples)
endif()

add_subdirectory(editor)
