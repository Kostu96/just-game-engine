#
# Copyright (C) 2021-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics_context_ogl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platform_win.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_base.hpp
)

set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
