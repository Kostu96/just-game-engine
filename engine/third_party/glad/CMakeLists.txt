#
# Copyright (C) 2021-2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

cmake_minimum_required(VERSION 3.16...3.24)

project(glad-proj
    VERSION 1.0
    LANGUAGES C)

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

add_library(glad::glad ALIAS glad)

if (GLAD_INSTALL)
    include(GNUInstallDirs)
    
    install(TARGETS glad
        EXPORT gladTargets
        INCLUDES DESTINATION include)
    
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/glad ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR DESTINATION include)
    
    install(EXPORT gladTargets
        NAMESPACE glad::
        FILE gladTargets.cmake
        DESTINATION lib/cmake/glad)
        
    include(CMakePackageConfigHelpers)
    
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/gladConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/gladConfig.cmake"
        INSTALL_DESTINATION "lib/cmake/glad"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO)
        
    write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/gladConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion)
        
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/gladConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/gladConfigVersion.cmake
        DESTINATION lib/cmake/glad)
        
    export(EXPORT gladTargets
        NAMESPACE glad::
        FILE "${CMAKE_CURRENT_BINARY_DIR}/gladTargets.cmake")
endif()
