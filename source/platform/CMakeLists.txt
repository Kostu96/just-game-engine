#
# Copyright (C) 2021-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(JNG_HEADERS_DIR ${JNG_HEADERS_DIR}/platform)

list(APPEND JNG_HEADERS
    ${JNG_HEADERS_DIR}/input.hpp
    ${JNG_HEADERS_DIR}/key_codes.hpp
    ${JNG_HEADERS_DIR}/mouse_codes.hpp
    ${JNG_HEADERS_DIR}/platform.hpp
    ${JNG_HEADERS_DIR}/window.hpp
)

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics_context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics_context.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics_context_ogl.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
)

if (WIN32)
    add_subdirectory(windows)
else()
    add_subdirectory(linux)
endif()

set(JNG_HEADERS ${JNG_HEADERS} PARENT_SCOPE)
set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
