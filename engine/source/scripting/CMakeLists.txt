#
# Copyright (C) 2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(JNG_HEADERS_DIR ${JNG_HEADERS_DIR}/scripting)

list(APPEND JNG_HEADERS
    ${JNG_HEADERS_DIR}/lua_engine.hpp
    ${JNG_HEADERS_DIR}/lua_script.hpp
    ${JNG_HEADERS_DIR}/native_script.hpp
)

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/lua_definitions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lua_definitions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lua_engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lua_script.cpp
)

set(JNG_HEADERS ${JNG_HEADERS} PARENT_SCOPE)
set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
