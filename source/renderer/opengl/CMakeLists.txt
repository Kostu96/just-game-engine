#
# Copyright (C) 2021-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers_ogl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers_ogl.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_api_impl_ogl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_api_impl_ogl.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_impl_ogl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_impl_ogl.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_impl_ogl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_impl_ogl.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_sources_ogl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_impl_ogl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_impl_ogl.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex_array_impl_ogl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex_array_impl_ogl.hpp
)

set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
