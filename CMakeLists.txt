#
# Copyright (C) 2020-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

cmake_minimum_required(VERSION 3.24)

### options: ###
set(JNG_VULKAN_SDK_DIR "" CACHE PATH "Path to your VulkanSDK installation directory")
option(JNG_BUILD_SAMPLES "Build sample projects" ON) # TODO: switch to off later on
################

project(jng-proj LANGUAGES CXX)
include(cmake/base_configuration.cmake)
include(cmake/compiler_warnings.cmake)

set(JNG_TARGET_NAME jng)

set(ENV{VULKAN_SDK} ${JNG_VULKAN_SDK_DIR})
find_package(Vulkan REQUIRED)
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
    IMGUI_USER_CONFIG="imconfig.hpp"
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
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

target_include_directories(${JNG_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm
)

target_include_directories(${JNG_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/jng
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${Vulkan_INCLUDE_DIRS}
)

#target_include_directories(${JNG_TARGET_NAME} PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/entt/include
#)

message(STATUS "____ ${Vulkan_LIBRARIES}")
target_link_libraries(${JNG_TARGET_NAME} PUBLIC
 #   Vulkan::Vulkan
 #   Vulkan::glslc
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
