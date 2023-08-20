#
# Copyright (C) 2022-2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

### Vulkan ###
find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    add_library(vulkan-shaderc SHARED IMPORTED GLOBAL)
    add_library(vulkan-spirv-cross-core STATIC IMPORTED GLOBAL)
    add_library(vulkan-spirv-cross-glsl STATIC IMPORTED GLOBAL)
    if (WIN32)
        set_target_properties(vulkan-shaderc PROPERTIES
            IMPORTED_LOCATION $ENV{VULKAN_SDK}/Bin/shaderc_shared.dll
            IMPORTED_IMPLIB $ENV{VULKAN_SDK}/Lib/shaderc_shared.lib
            IMPORTED_LOCATION_DEBUG $ENV{VULKAN_SDK}/Bin/shaderc_sharedd.dll
            IMPORTED_IMPLIB_DEBUG $ENV{VULKAN_SDK}/Lib/shaderc_sharedd.lib
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
    endif()
    
    list(APPEND Vulkan_LIBRARIES
        vulkan-shaderc
        vulkan-spirv-cross-core
        vulkan-spirv-cross-glsl
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
add_subdirectory(box2d)
##############

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

### Lua ###
add_subdirectory(lua)
###########

### msdf-atlas-gen ###
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory msdf_atlas_gen_build)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR}/engine/third_party/msdf-atlas-gen -B msdf_atlas_gen_build -D MSDF_ATLAS_INSTALL=ON)
execute_process(COMMAND ${CMAKE_COMMAND} --build msdf_atlas_gen_build --config Debug)
execute_process(COMMAND ${CMAKE_COMMAND} --build msdf_atlas_gen_build --config Release)
execute_process(COMMAND ${CMAKE_COMMAND} --install msdf_atlas_gen_build --prefix msdf_atlas_gen_install)
set(ENV{msdf-atlas-gen_DIR} ${CMAKE_BINARY_DIR}/msdf_atlas_gen_install)
set(ENV{msdfgen_DIR} ${CMAKE_BINARY_DIR}/msdf_atlas_gen_install)
set(ENV{freetype_DIR} ${CMAKE_BINARY_DIR}/msdf_atlas_gen_install)
find_package(msdf-atlas-gen REQUIRED)
#add_subdirectory(msdf-atlas-gen)
######################

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

set(JNG_ENGINE_THIRD_PARTY_LIBRARIES
    box2d
    glad
    glfw
    imgui
    lua
    spdlog
    stb
    yaml-cpp
)

set_target_properties(
    ${JNG_ENGINE_THIRD_PARTY_LIBRARIES}
    update_mappings
    PROPERTIES
    FOLDER third_party
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    EXCLUDE_FROM_ALL true
)

set(JNG_ENGINE_THIRD_PARTY_LIBRARIES
    ${Vulkan_LIBRARIES}
    ${JNG_ENGINE_THIRD_PARTY_LIBRARIES}
PARENT_SCOPE)
