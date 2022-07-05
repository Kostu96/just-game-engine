#
# Copyright (C) 2021-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/error_checks_macros_win.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/error_checks_macros_win.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics_context_d3d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics_context_d3d.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics_context_ogl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_base.hpp
)

set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
