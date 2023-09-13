#
# Copyright (C) 2020-2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

cmake_minimum_required(VERSION 3.16...3.24)

project(just-game-engine 
    VERSION 0.1.0
    DESCRIPTION "Hobby game engine"
    LANGUAGES CXX
)

include(CMakePackageConfigHelpers)
include(scripts/cmake/base_configuration.cmake)
include(scripts/cmake/compiler_warnings.cmake)

### options: ###
option(JNG_BUILD_SAMPLES "Build sample projects" ON)
option(JNG_INSTALL "Do not generate install target" ${JNG_MAIN_PROJECT})
################

find_package(box2d REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Lua CONFIG REQUIRED)
find_package(msdf-atlas-gen REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(glad REQUIRED)
find_package(stb REQUIRED)

add_subdirectory(engine)
add_subdirectory(editor)

if (JNG_BUILD_SAMPLES)
    message(STATUS "Building sample projects")
    add_subdirectory(samples)
endif()
