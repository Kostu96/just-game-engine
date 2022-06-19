#
# Copyright (C) 2021-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

list(APPEND K2D_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/components.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene.hpp
)

set(K2D_SOURCES ${K2D_SOURCES} PARENT_SCOPE)
