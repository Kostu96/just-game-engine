#
# Copyright (C) 2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_layer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_layer.hpp
)

set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
