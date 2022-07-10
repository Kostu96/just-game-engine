#
# Copyright (C) 2021-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(JNG_HEADERS_DIR ${JNG_HEADERS_DIR}/renderer)

list(APPEND JNG_HEADERS
    ${JNG_HEADERS_DIR}/buffers.hpp
    ${JNG_HEADERS_DIR}/camera.hpp
    ${JNG_HEADERS_DIR}/framebuffer.hpp
    ${JNG_HEADERS_DIR}/renderer.hpp
    ${JNG_HEADERS_DIR}/renderer_api.hpp
    ${JNG_HEADERS_DIR}/renderer2d.hpp
    ${JNG_HEADERS_DIR}/shader.hpp
    ${JNG_HEADERS_DIR}/texture.hpp
    ${JNG_HEADERS_DIR}/vertex_array.hpp
)

list(APPEND JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framebuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_api.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer2d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex_array.cpp
)

if(WIN32)
    add_subdirectory(direct3d)
endif()
#add_subdirectory(vulkan)
add_subdirectory(opengl)

set(JNG_HEADERS ${JNG_HEADERS} PARENT_SCOPE)
set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
