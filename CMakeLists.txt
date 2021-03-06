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

add_subdirectory(third_party)

set(JNG_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/jng)
add_subdirectory(source)

set(JNG_SHADER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/basic_fragment.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/basic_vertex.glsl
)

add_library(${JNG_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${JNG_HEADERS}
    ${JNG_SOURCES}
    ${JNG_SHADER_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/jng PREFIX source FILES ${JNG_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${JNG_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders PREFIX shaders FILES ${JNG_SHADER_SOURCES})

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

target_include_directories(${JNG_TARGET_NAME} SYSTEM
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/entt/include
    PRIVATE
    ${Vulkan_INCLUDE_DIRS}
)

target_include_directories(${JNG_TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/jng
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

if (WIN32)
    target_link_options(${JNG_TARGET_NAME} PUBLIC
        /ignore:4099
    )
endif()

target_link_libraries(${JNG_TARGET_NAME} PUBLIC ${JNG_THIRD_PARTY_LIBRARIES})

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
