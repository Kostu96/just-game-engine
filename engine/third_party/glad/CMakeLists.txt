#
# Copyright (C) 2021-2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

cmake_minimum_required(VERSION 3.20)

project(glad-proj LANGUAGES C)
set(GLAD_TARGET_NAME glad)

add_library(${GLAD_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/gl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/wgl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/wgl.c
)

target_include_directories(${GLAD_TARGET_NAME} SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
