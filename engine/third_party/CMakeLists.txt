#
# Copyright (C) 2022-2023 Konstanty Misiak
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

### glad: ###
set(GLAD_INSTALL ON CACHE BOOL "" FORCE)
add_subdirectory(glad)
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

### ImGui: ###
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
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h)

if (WIN32)
    list(APPEND IMGUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_dx11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_dx11.h)
endif()

add_library(imgui STATIC ${IMGUI_SOURCES})

target_compile_options(imgui PRIVATE $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_FLAGS}>)

target_include_directories(imgui SYSTEM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(imgui PUBLIC glfw)
##############

### spdlog: ###
add_subdirectory(spdlog)
target_compile_options(spdlog PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_FLAGS}>
)
###############

### stb: ###
set(STB_INSTALL ON CACHE BOOL "" FORCE)
add_subdirectory(stb)
############

set(JNG_ENGINE_THIRD_PARTY_LIBRARIES
    glfw
    imgui
    spdlog
)

set_target_properties(
    ${JNG_ENGINE_THIRD_PARTY_LIBRARIES}
    glad
    stb
    update_mappings
    PROPERTIES
    FOLDER third_party
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    EXCLUDE_FROM_ALL true
)

set(JNG_ENGINE_THIRD_PARTY_LIBRARIES
    ${JNG_ENGINE_THIRD_PARTY_LIBRARIES}
PARENT_SCOPE)
