#
# Copyright (C) 2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(JNG_HEADERS_DIR ${JNG_HEADERS_DIR}/utilities)

list(APPEND JNG_HEADERS
    ${JNG_HEADERS_DIR}/math.hpp
)

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/file.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/file.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math.cpp
)

set(JNG_HEADERS ${JNG_HEADERS} PARENT_SCOPE)
set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
