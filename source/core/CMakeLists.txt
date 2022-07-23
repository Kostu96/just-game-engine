#
# Copyright (C) 2020-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(JNG_HEADERS_DIR ${JNG_HEADERS_DIR}/core)

list(APPEND JNG_HEADERS
    ${JNG_HEADERS_DIR}/base.hpp
    ${JNG_HEADERS_DIR}/engine.hpp
    ${JNG_HEADERS_DIR}/entry_point.hpp
    ${JNG_HEADERS_DIR}/event.hpp
    ${JNG_HEADERS_DIR}/GUID.hpp
    ${JNG_HEADERS_DIR}/key_events.hpp
    ${JNG_HEADERS_DIR}/layer.hpp
    ${JNG_HEADERS_DIR}/layer_stack.hpp
    ${JNG_HEADERS_DIR}/mouse_events.hpp
    ${JNG_HEADERS_DIR}/window_events.hpp
)

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/base_internal.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GUID.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/layer_stack.cpp
)

set(JNG_HEADERS ${JNG_HEADERS} PARENT_SCOPE)
set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
