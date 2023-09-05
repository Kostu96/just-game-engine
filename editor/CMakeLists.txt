#
# Copyright (C) 2022-2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

set(EDITOR_TARGET_NAME jng-editor)

add_subdirectory(third_party)

add_subdirectory(source)

add_executable(${EDITOR_TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${EDITOR_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${EDITOR_SOURCES})

set_target_warnings(${EDITOR_TARGET_NAME})

target_compile_options(${EDITOR_TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/MP /fp:fast /external:anglebrackets /external:W1>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${EDITOR_TARGET_NAME} PRIVATE
    ${JNG_TARGET_NAME}
    ${JNG_EDITOR_THIRD_PARTY_LIBRARIES}
)

set_target_properties(${EDITOR_TARGET_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${EDITOR_TARGET_NAME}>
)

add_custom_command(
    TARGET ${EDITOR_TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${EDITOR_TARGET_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/engine/assets $<TARGET_FILE_DIR:${EDITOR_TARGET_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/engine/shaders $<TARGET_FILE_DIR:${EDITOR_TARGET_NAME}>/assets/shaders
)
