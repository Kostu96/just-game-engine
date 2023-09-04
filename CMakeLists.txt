#
# Copyright (C) 2020-2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

cmake_minimum_required(VERSION 3.16...3.24)

project(just-game-engine 
    VERSION 0.1
    DESCRIPTION "Hobby game engine"
    LANGUAGES CXX
)

include(CMakePackageConfigHelpers)
include(cmake/base_configuration.cmake)
include(cmake/compiler_warnings.cmake)

### options: ###
option(JNG_BUILD_SAMPLES "Build sample projects" ON)
option(JNG_NO_INSTALL "Do not generate install target" OFF)
################

if (NOT DEFINED CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX "d")
endif()

add_subdirectory(engine)
add_subdirectory(editor)

if (JNG_BUILD_SAMPLES)
    message(STATUS "Building sample projects")
    add_subdirectory(samples)
endif()
