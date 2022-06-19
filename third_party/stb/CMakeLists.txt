cmake_minimum_required(VERSION 3.16)

project(stb-proj LANGUAGES CXX)

set(TARGET_NAME stb)

add_library(${TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/stb/stb_image.h
)

target_include_directories(${TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
