#
# Copyright (C) 2021-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers_d3d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers_d3d.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framebuffer_d3d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framebuffer_d3d.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_api_d3d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_api_d3d.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_d3d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_d3d.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_d3d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_d3d.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex_array_d3d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex_array_d3d.hpp
)

set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
