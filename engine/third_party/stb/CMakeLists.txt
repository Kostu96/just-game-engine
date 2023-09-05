cmake_minimum_required(VERSION 3.16...3.24)

project(stb-proj LANGUAGES CXX)

set(STB_MAIN_PROJECT OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(STB_MAIN_PROJECT ON)
endif()
set(CMAKE_DEBUG_POSTFIX "d")

option(STB_INSTALL "Generate install target" ${GLAD_MAIN_PROJECT})

add_library(stb STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/stb/stb_image.h)

target_include_directories(stb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

add_library(stb::stb ALIAS stb)

if (${STB_INSTALL})
    include(GNUInstallDirs)
    
    install(TARGETS stb
        EXPORT stbTargets
        INCLUDES DESTINATION include)
    
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/stb DESTINATION include)
    
    install(EXPORT stbTargets
        NAMESPACE stb::
        FILE stbTargets.cmake
        DESTINATION lib/cmake/stb)
        
    include(CMakePackageConfigHelpers)
    
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/stbConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/stbConfig.cmake"
        INSTALL_DESTINATION "lib/cmake/stb"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO)
        
    write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/stbConfigVersion.cmake"
        COMPATIBILITY AnyNewerVersion)
        
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/stbConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/stbConfigVersion.cmake
        DESTINATION lib/cmake/stb)
        
    export(EXPORT stbTargets
        NAMESPACE stb::
        FILE "${CMAKE_CURRENT_BINARY_DIR}/stbTargets.cmake")
endif()
