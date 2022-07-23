#
# Copyright (C) 2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(EDITOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_context.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_layer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor_layer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(widgets)

set(EDITOR_SOURCES ${EDITOR_SOURCES} PARENT_SCOPE)
