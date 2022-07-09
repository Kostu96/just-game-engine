#
# Copyright (C) 2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(SAMPLE_TARGET_NAME hello_framebuffer)

set(SAMPLE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${SAMPLE_TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/fragment.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/vertex.glsl
    ${SAMPLE_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SAMPLE_SOURCES})

set_target_warnings(${SAMPLE_TARGET_NAME})

target_compile_options(${SAMPLE_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${SAMPLE_TARGET_NAME} PRIVATE
    ${JNG_TARGET_NAME}
)

set_target_properties(${SAMPLE_TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${SAMPLE_TARGET_NAME}>
    FOLDER samples
)

add_custom_command(
    TARGET ${SAMPLE_TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${SAMPLE_TARGET_NAME}>/assets/${SAMPLE_TARGET_NAME}
)
