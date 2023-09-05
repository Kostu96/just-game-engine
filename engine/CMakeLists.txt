#
# Copyright (C) 2022-2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(JNG_TARGET_NAME jng)
set(JNG_TARGET_NAME ${JNG_TARGET_NAME} PARENT_SCOPE)

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

add_library(${JNG_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${JNG_HEADERS}
    ${JNG_SOURCES}
    ${JNG_SHADER_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/jng PREFIX source FILES ${JNG_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${JNG_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/shaders PREFIX shaders FILES ${JNG_SHADER_SOURCES})

target_compile_definitions(${JNG_TARGET_NAME} PUBLIC
    $<$<CONFIG:Debug>:JNG_DEBUG>
    IMGUI_USER_CONFIG="jng/imconfig.hpp"
)
if (WIN32)
    target_compile_definitions(${JNG_TARGET_NAME} PUBLIC JNG_WINDOWS)
endif()

set_target_warnings(${JNG_TARGET_NAME})

target_compile_options(${JNG_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>)

target_include_directories(${JNG_TARGET_NAME} SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/entt/include)

target_include_directories(${JNG_TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/jng
    ${CMAKE_CURRENT_SOURCE_DIR}/source)

target_link_libraries(${JNG_TARGET_NAME}
    PUBLIC
    glm::glm
    yaml-cpp::yaml-cpp
    ${JNG_ENGINE_THIRD_PARTY_LIBRARIES}
    PRIVATE
    msdf-atlas-gen::msdf-atlas-gen
    VulkanShaderTools)

target_precompile_headers(${JNG_TARGET_NAME} PUBLIC
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

set_target_properties(${JNG_TARGET_NAME} PROPERTIES
    VERSION "${PROJECT_VERSION}"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

add_library(${JNG_TARGET_NAME}::${JNG_TARGET_NAME} ALIAS ${JNG_TARGET_NAME}) # NOTE: this is to ensure the same targets are exported with add_subdirectory and find_package

#set(CONFIG_EXPORT_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${JNG_TARGET_NAME}")
#set(EXPORT_TARGETS ${JNG_TARGET_NAME})
#configure_package_config_file(
#    "${PROJECT_SOURCE_DIR}/${JNG_TARGET_NAME}-config.cmake.in"
#    "${PROJECT_BINARY_DIR}/${JNG_TARGET_NAME}-config.cmake"
#    INSTALL_DESTINATION "${CONFIG_EXPORT_DIR}"
#    #PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR CONFIG_EXPORT_DIR
#)
#unset(EXPORT_TARGETS)
#
#write_basic_package_version_file(
#    "${PROJECT_BINARY_DIR}/${JNG_TARGET_NAME}-config-version.cmake"
#    COMPATIBILITY AnyNewerVersion
#)
#
#if (NOT ${JNG_NO_INSTALL})
#    install(TARGETS ${JNG_TARGET_NAME}
#        EXPORT ${JNG_TARGET_NAME}-targets
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib
#        RUNTIME DESTINATION bin
#        INCLUDES DESTINATION include
#    )
#    
#    install(EXPORT ${JNG_TARGET_NAME}-targets
#        NAMESPACE ${JNG_TARGET_NAME}::
#        DESTINATION "${CONFIG_EXPORT_DIR}"
#    )
#endif()
