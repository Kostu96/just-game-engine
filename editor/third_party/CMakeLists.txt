#
# Copyright (C) 2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

### ImGuizmo ###
set(IMGUIZMO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo/ImGuizmo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo/ImGuizmo.h
)

add_library(imguizmo STATIC ${IMGUIZMO_SOURCES})

target_compile_options(imguizmo PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_FLAGS}>
)

target_include_directories(imguizmo SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo
)

target_link_libraries(imguizmo PUBLIC
    imgui
)
################

set(JNG_EDITOR_THIRD_PARTY_LIBRARIES
    imguizmo
)

set_target_properties(
    ${JNG_EDITOR_THIRD_PARTY_LIBRARIES}
    PROPERTIES
    FOLDER third_party
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    EXCLUDE_FROM_ALL true
)

set(JNG_EDITOR_THIRD_PARTY_LIBRARIES ${JNG_EDITOR_THIRD_PARTY_LIBRARIES} PARENT_SCOPE)
