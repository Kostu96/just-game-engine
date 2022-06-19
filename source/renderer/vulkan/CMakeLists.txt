#
# Copyright (C) 2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers_impl_vlk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers_impl_vlk.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_api_impl_vlk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_api_impl_vlk.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_impl_vlk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_impl_vlk.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_sources_vlk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_impl_vlk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_impl_vlk.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex_array_impl_vlk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex_array_impl_vlk.hpp
)

set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
