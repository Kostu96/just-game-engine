#
# Copyright (C) 2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(JNG_TARGET_NAME jng)
set(JNG_TARGET_NAME ${JNG_TARGET_NAME} PARENT_SCOPE)

add_subdirectory(third_party)

set(JNG_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/jng)
add_subdirectory(source)

set(JNG_SHADER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/basic_fragment.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/basic_vertex.glsl
)

add_library(${JNG_TARGET_NAME} STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${JNG_HEADERS}
    ${JNG_SOURCES}
    ${JNG_SHADER_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/jng PREFIX source FILES ${JNG_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${JNG_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/shaders PREFIX shaders FILES ${JNG_SHADER_SOURCES})

target_compile_definitions(${JNG_TARGET_NAME} PUBLIC
    $<$<CONFIG:Debug>:JNG_DEBUG>
    IMGUI_USER_CONFIG="jng/imconfig.hpp"
)
if (WIN32)
    target_compile_definitions(${JNG_TARGET_NAME} PUBLIC
        JNG_WINDOWS
    )
endif()

set_target_warnings(${JNG_TARGET_NAME})

target_compile_options(${JNG_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
)

target_include_directories(${JNG_TARGET_NAME} SYSTEM
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/entt/include
    PRIVATE
    ${Vulkan_INCLUDE_DIRS}
)

target_include_directories(${JNG_TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/jng
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

if (WIN32)
    target_link_options(${JNG_TARGET_NAME} PUBLIC
        /ignore:4099
    )
endif()

message(STATUS ${JNG_ENGINE_THIRD_PARTY_LIBRARIES})
target_link_libraries(${JNG_TARGET_NAME} PUBLIC ${JNG_ENGINE_THIRD_PARTY_LIBRARIES})
