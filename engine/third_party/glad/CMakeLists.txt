#
# Copyright (C) 2021-2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

cmake_minimum_required(VERSION 3.16...3.24)

project(glad-proj LANGUAGES C)

set(GLAD_MAIN_PROJECT OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(GLAD_MAIN_PROJECT ON)
endif()
set(CMAKE_DEBUG_POSTFIX "d")

option(GLAD_INSTALL "Generate install target" ${GLAD_MAIN_PROJECT})

set(GLAD_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/gl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/wgl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h)

add_library(glad STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${GLAD_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/wgl.c)

target_include_directories(glad SYSTEM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

set_target_properties(glad PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

if (GLAD_INSTALL)
    install(TARGETS glad
            EXPORT gladTargets
            DESTINATION lib)
            
    install(FILES ${GLAD_HEADERS}
            DESTINATION include)
endif()
