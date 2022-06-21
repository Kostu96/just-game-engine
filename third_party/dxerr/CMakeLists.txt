#
# Copyright (C) 2021-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

cmake_minimum_required(VERSION 3.20)

project(dxerr-proj LANGUAGES CXX)
set(DXERR_TARGET_NAME dxerr)

add_library(${DXERR_TARGET_NAME} STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dxerr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dxerr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DXGetErrorDescription.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DXGetErrorString.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DXTrace.inl
)

target_include_directories(${DXERR_TARGET_NAME} SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
