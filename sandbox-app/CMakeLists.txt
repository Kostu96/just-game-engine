#
# Copyright (C) 2020-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(SANDBOX_TARGET_NAME sandbox-app)

set(SANDBOX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/camera_controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/camera_controller.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/game_data.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/game_layer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/game_layer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/subtexture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/subtexture.hpp
)

add_executable(${SANDBOX_TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${SANDBOX_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${SANDBOX_SOURCES})

k2d_set_target_warnings(${SANDBOX_TARGET_NAME})

target_compile_options(${SANDBOX_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${SANDBOX_TARGET_NAME} PRIVATE
    ${K2D_TARGET_NAME}
)

set_target_properties(${SANDBOX_TARGET_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${SANDBOX_TARGET_NAME}>
)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${SANDBOX_TARGET_NAME})
endif()

add_custom_command(
    TARGET ${SANDBOX_TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${SANDBOX_TARGET_NAME}>/assets
)
