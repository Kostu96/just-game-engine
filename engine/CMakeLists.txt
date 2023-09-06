#
# Copyright (C) 2022-2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

add_subdirectory(third_party)

set(JNG_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/jng)
add_subdirectory(source)

set(JNG_SHADER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/circle_fragment.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/circle_vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/line_fragment.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/line_vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/quad_fragment.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/quad_vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/text_fragment.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/text_vertex.glsl)

add_library(jng STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${JNG_HEADERS}
    ${JNG_SOURCES}
    ${JNG_SHADER_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/jng PREFIX source FILES ${JNG_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${JNG_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/shaders PREFIX shaders FILES ${JNG_SHADER_SOURCES})

target_compile_definitions(jng PUBLIC
    $<$<CONFIG:Debug>:JNG_DEBUG>
    IMGUI_USER_CONFIG="jng/imconfig.hpp"
)
if (WIN32)
    target_compile_definitions(jng PUBLIC JNG_WINDOWS)
endif()

set_target_warnings(jng)

target_compile_options(jng PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>)

target_include_directories(jng SYSTEM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/entt/include>
    $<INSTALL_INTERFACE:include>)

target_include_directories(jng
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/jng
    ${CMAKE_CURRENT_SOURCE_DIR}/source)

target_link_libraries(jng
    PUBLIC
    glm::glm
    imgui
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    PRIVATE
    box2d::box2d
    glad::glad
    glfw
    lua::lua
    msdf-atlas-gen::msdf-atlas-gen
    stb::stb
    VulkanShaderTools)

target_precompile_headers(jng PUBLIC
    <glm/glm.hpp>
    <array>
    <cstdint>
    <filesystem>
    <fstream>
    <map>
    <memory>
    <string>
    <unordered_map>
    <vector>)

set_target_properties(jng PROPERTIES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

add_library(jng::jng ALIAS jng)

if (${JNG_INSTALL})
    include(GNUInstallDirs)
    
    install(TARGETS jng imgui VulkanShaderTools
        EXPORT jngTargets
        INCLUDES DESTINATION include)
    
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/jng DESTINATION include)
    
    install(EXPORT jngTargets
        NAMESPACE jng::
        FILE jngTargets.cmake
        DESTINATION lib/cmake/jng)
        
    include(CMakePackageConfigHelpers)
    
    configure_package_config_file(${CMAKE_SOURCE_DIR}/jngConfig.cmake.in "${CMAKE_BINARY_DIR}/jngConfig.cmake"
        INSTALL_DESTINATION "lib/cmake/jng"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO)
        
    write_basic_package_version_file("${CMAKE_BINARY_DIR}/jngConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY SameMajorVersion)
        
    install(FILES
        ${CMAKE_BINARY_DIR}/jngConfig.cmake
        ${CMAKE_BINARY_DIR}/jngConfigVersion.cmake
        DESTINATION lib/cmake/jng)
        
    export(EXPORT jngTargets
        NAMESPACE jng::
        FILE "${CMAKE_BINARY_DIR}/jngTargets.cmake")
endif()
