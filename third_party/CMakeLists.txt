#
# Copyright (C) 2020-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

### disable third_party messages and static_analysis
function(message)
    if (NOT MESSAGE_QUIET)
        _message(${ARGN})
    endif()
endfunction()
set(MESSAGE_QUIET ON)
set(CMAKE_CXX_CLANG_TIDY "")
set(CMAKE_CXX_CPPCHECK "")

set(COMPILER_FLAGS /MP /fp:fast)

### dxerr: ###
if (WIN32)
    add_subdirectory(dxerr)
endif()
###########

### cpp-common-lib ###
add_subdirectory(cpp-common-lib)
######################

### glad: ###
add_subdirectory(glad)
target_compile_options(glad PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_FLAGS}>
)
#############

### glfw: ###
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
target_compile_options(glfw PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_FLAGS}>
)
#############

### imgui: ###
set(IMGUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3_loader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h
)

if (WIN32)
    list(APPEND IMGUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_dx11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_dx11.h
    )
endif()

add_library(imgui STATIC ${IMGUI_SOURCES})

target_compile_options(imgui PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_FLAGS}>
)

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

target_link_libraries(imgui PUBLIC
    glfw
)
##############

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
    imgui
    spdlog
    stb
    PROPERTIES
    FOLDER third_party
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    EXCLUDE_FROM_ALL true
)

if (WIN32)
    set_target_properties(
        dxerr
        PROPERTIES
        FOLDER third_party
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
        EXCLUDE_FROM_ALL true
    )
endif()
