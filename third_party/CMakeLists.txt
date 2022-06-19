#
# Copyright (C) 2020-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

### cpp-common-lib ###
add_subdirectory(cpp-common-lib)
######################

### glad: ###
add_subdirectory(glad)
target_compile_options(glad PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_FLAGS}>
)
#############

### GLFW: ###
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
target_compile_options(glfw PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_FLAGS}>
)
#############

### spdlog: ###
add_subdirectory(spdlog)
target_compile_options(spdlog PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_FLAGS}>
)
###############

### stb: ###
add_subdirectory(stb)
target_compile_options(stb PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_FLAGS}>
)
############

set_target_properties(
    cpp-common-lib
    glad
    glfw
    update_mappings
#    imgui
    spdlog
    stb
    PROPERTIES
    FOLDER third_party
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    EXCLUDE_FROM_ALL true
)

#if (WIN32)
#    set_target_properties(
#        dxerr
#        PROPERTIES
#        FOLDER third_party
#        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
#        EXCLUDE_FROM_ALL true
#    )
#endif()
