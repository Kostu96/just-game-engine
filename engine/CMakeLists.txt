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
    stb::stb)

target_precompile_headers(jng PRIVATE
    <glm/glm.hpp>
    <array>
    <chrono>
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
    
    install(TARGETS jng imgui
        EXPORT jngTargets
        INCLUDES DESTINATION include)
    
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION bin/assets)
    
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/jng DESTINATION include)
    install(DIRECTORY ${glm_DIR}/../../../include/glm  DESTINATION include)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui/imgui.h DESTINATION include)
    
    file(TO_CMAKE_PATH $ENV{FREETYPE_ROOT} FREETYPE_ROOT2)
    install(FILES
        ${box2d_DIR}/../../../lib/box2d$<$<CONFIG:Debug>:d>.lib
        ${glad_DIR}/../../../lib/glad$<$<CONFIG:Debug>:d>.lib
        ${glfw3_DIR}/../../../lib/glfw3.lib
        ${Lua_DIR}/../../../lib/lua$<$<CONFIG:Debug>:d>.lib
        ${msdf-atlas-gen_DIR}/../../../lib/msdf-atlas-gen$<$<CONFIG:Debug>:d>.lib
        ${msdfgen_DIR}/../../../lib/msdfgen-core$<$<CONFIG:Debug>:d>.lib
        ${msdfgen_DIR}/../../../lib/msdfgen-ext$<$<CONFIG:Debug>:d>.lib
        ${spdlog_DIR}/../../../lib/spdlog$<$<CONFIG:Debug>:d>.lib
        ${stb_DIR}/../../../lib/stb$<$<CONFIG:Debug>:d>.lib
        ${yaml-cpp_DIR}/../../../lib/yaml-cpp$<$<CONFIG:Debug>:d>.lib
        ${FREETYPE_ROOT2}/lib/freetype$<$<CONFIG:Debug>:d>.lib
        DESTINATION lib)
    
    install(FILES
        ${CMAKE_SOURCE_DIR}/README.md
        ${CMAKE_SOURCE_DIR}/LICENSE
        DESTINATION .)
    
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
