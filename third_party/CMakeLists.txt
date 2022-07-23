#
# Copyright (C) 2020-2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

### Vulkan ###
find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    add_library(vulkan-shaderc STATIC IMPORTED GLOBAL)
    add_library(vulkan-spirv-cross-core STATIC IMPORTED GLOBAL)
    add_library(vulkan-spirv-cross-glsl STATIC IMPORTED GLOBAL)
    add_library(vulkan-spirv-cross-hlsl STATIC IMPORTED GLOBAL)
    if (WIN32)
        set_target_properties(vulkan-shaderc PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/shaderc_combined.lib
            IMPORTED_LOCATION_DEBUG $ENV{VULKAN_SDK}/Lib/shaderc_combinedd.lib
            IMPORTED_CONFIGURATIONS "Release;Debug"
        )
        set_target_properties(vulkan-spirv-cross-core PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/spirv-cross-core.lib
            IMPORTED_LOCATION_DEBUG $ENV{VULKAN_SDK}/Lib/spirv-cross-cored.lib
            IMPORTED_CONFIGURATIONS "Release;Debug"
        )
        set_target_properties(vulkan-spirv-cross-glsl PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/spirv-cross-glsl.lib
            IMPORTED_LOCATION_DEBUG $ENV{VULKAN_SDK}/Lib/spirv-cross-glsld.lib
            IMPORTED_CONFIGURATIONS "Release;Debug"
        )
        set_target_properties(vulkan-spirv-cross-hlsl PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/Lib/spirv-cross-hlsl.lib
            IMPORTED_LOCATION_DEBUG $ENV{VULKAN_SDK}/Lib/spirv-cross-hlsld.lib
            IMPORTED_CONFIGURATIONS "Release;Debug"
        )
    else()
        set_target_properties(vulkan-shaderc PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/lib/libshaderc_combined.a
            IMPORTED_CONFIGURATIONS "Release"
        )
        set_target_properties(vulkan-spirv-cross-core PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/lib/libspirv-cross-core.a
            IMPORTED_CONFIGURATIONS "Release"
        )
        set_target_properties(vulkan-spirv-cross-glsl PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/lib/libspirv-cross-glsl.a
            IMPORTED_CONFIGURATIONS "Release"
        )
        set_target_properties(vulkan-spirv-cross-hlsl PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/lib/libspirv-cross-hlsl.a
            IMPORTED_CONFIGURATIONS "Release"
        )
    endif()
    
    list(APPEND Vulkan_LIBRARIES
        vulkan-shaderc
        vulkan-spirv-cross-core
        vulkan-spirv-cross-glsl
        vulkan-spirv-cross-hlsl
    )
    set(Vulkan_INCLUDE_DIRS ${Vulkan_INCLUDE_DIRS} PARENT_SCOPE)
    set(Vulkan_LIBRARIES ${Vulkan_LIBRARIES} PARENT_SCOPE)
endif()
##############

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

### box2d: ###
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
add_subdirectory(box2d)
##############

### dxerr: ###
if (WIN32)
    add_subdirectory(dxerr)
endif()
##############

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

target_include_directories(imgui SYSTEM PUBLIC
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

### yaml-cpp: ###
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
add_subdirectory(yaml-cpp)
target_compile_options(yaml-cpp PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:${COMPILER_FLAGS}>
)
target_compile_definitions(yaml-cpp PUBLIC
    YAML_CPP_STATIC_DEFINE
)
#################

set_target_properties(
    box2d
    cpp-common-lib
    glad
    glfw
    update_mappings
    imgui
    spdlog
    stb
    yaml-cpp
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

set(JNG_THIRD_PARTY_LIBRARIES
    ${Vulkan_LIBRARIES}
    box2d
    cpp-common-lib
    glad
    glfw
    imgui
    spdlog
    stb
    yaml-cpp
PARENT_SCOPE)
