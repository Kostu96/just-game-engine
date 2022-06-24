#
# Copyright (C) 2020-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(JNG_HEADERS
    ${JNG_HEADERS_DIR}/jng.hpp
    ${JNG_HEADERS_DIR}/imconfig.hpp
)

set(JNG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
)

add_subdirectory(core)
add_subdirectory(debug)
add_subdirectory(platform)
add_subdirectory(renderer)
#add_subdirectory(scene)

set(JNG_HEADERS ${JNG_HEADERS} PARENT_SCOPE)
set(JNG_SOURCES ${JNG_SOURCES} PARENT_SCOPE)
