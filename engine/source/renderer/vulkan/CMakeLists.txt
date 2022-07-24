#
# Copyright (C) 2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers_vlk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers_vlk.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_api_vlk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_api_vlk.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_vlk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_vlk.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_vlk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_vlk.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex_array_vlk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex_array_vlk.hpp
)

set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
