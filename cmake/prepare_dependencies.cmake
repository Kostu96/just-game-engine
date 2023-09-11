#
# Copyright (C) 2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

find_package(Git QUIET REQUIRED)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies OUTPUT_QUIET)

message(STATUS "Preparing box2d...")
if (NOT EXISTS "${PROJECT_BINARY_DIR}/dependencies/box2d-fork/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/Kostu96/box2d-fork.git WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies OUTPUT_QUIET)
    execute_process(COMMAND ${GIT_EXECUTABLE} checkout 8c6a8a1 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies/box2d-fork OUTPUT_QUIET)
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/box2d-fork/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${PROJECT_BINARY_DIR}/dependencies/box2d-fork -B ${PROJECT_BINARY_DIR}/dependencies/box2d-fork/build -DBOX2D_BUILD_UNIT_TESTS=OFF -DBOX2D_BUILD_TESTBED=OFF -DBOX2D_BUILD_DOCS=OFF OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/box2d-fork/build --config Debug OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/box2d-fork/build --config Release OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/box2d-fork/install OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/box2d-fork/install OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/box2d-fork/build --config Debug --prefix "${PROJECT_BINARY_DIR}/dependencies/box2d-fork/install" OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/box2d-fork/build --config Release --prefix "${PROJECT_BINARY_DIR}/dependencies/box2d-fork/install" OUTPUT_QUIET)
set(box2d_DIR ${PROJECT_BINARY_DIR}/dependencies/box2d-fork/install/lib/cmake/box2d)
find_package(box2d REQUIRED)

message(STATUS "Preparing glfw...")
if (NOT EXISTS "${PROJECT_BINARY_DIR}/dependencies/glfw/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/glfw/glfw.git WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies OUTPUT_QUIET)
    execute_process(COMMAND ${GIT_EXECUTABLE} checkout 3eaf125 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies/glfw OUTPUT_QUIET)
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/glfw/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${PROJECT_BINARY_DIR}/dependencies/glfw -B ${PROJECT_BINARY_DIR}/dependencies/glfw/build -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/glfw/build --config Debug OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/glfw/build --config Release OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/glfw/install OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/glfw/install OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/glfw/build --config Debug --prefix "${PROJECT_BINARY_DIR}/dependencies/glfw/install" OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/glfw/build --config Release --prefix "${PROJECT_BINARY_DIR}/dependencies/glfw/install" OUTPUT_QUIET)
set(glfw3_DIR ${PROJECT_BINARY_DIR}/dependencies/glfw/install/lib/cmake/glfw3)
find_package(glfw3 REQUIRED)

message(STATUS "Preparing glm...")
if (NOT EXISTS "${PROJECT_BINARY_DIR}/dependencies/glm/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/g-truc/glm.git WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies OUTPUT_QUIET)
    execute_process(COMMAND ${GIT_EXECUTABLE} checkout 47585fd WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies/glm OUTPUT_QUIET)
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/glm/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${PROJECT_BINARY_DIR}/dependencies/glm -B ${PROJECT_BINARY_DIR}/dependencies/glm/build -DBUILD_TESTING=OFF OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/glm/install OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/glm/build --prefix "${PROJECT_BINARY_DIR}/dependencies/glm/install" OUTPUT_QUIET)
set(glm_DIR ${PROJECT_BINARY_DIR}/dependencies/glm/install/lib/cmake/glm)
find_package(glm REQUIRED)

message(STATUS "Preparing lua...")
if (NOT EXISTS "${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/Kostu96/lua-5.4.4.git WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies OUTPUT_QUIET)
    execute_process(COMMAND ${GIT_EXECUTABLE} checkout 9ebf101 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4 OUTPUT_QUIET)
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4 -B ${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/build --config Debug OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/build --config Release OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/install OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/install OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/build --config Debug --prefix "${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/install" OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/build --config Release --prefix "${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/install" OUTPUT_QUIET)
set(Lua_DIR ${PROJECT_BINARY_DIR}/dependencies/lua-5.4.4/install/lib/cmake/lua)
find_package(Lua CONFIG REQUIRED)

message(STATUS "Preparing msdf-atlas-gen...")
if (NOT EXISTS "${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/Kostu96/msdf-atlas-gen-fork.git --depth 1 --shallow-submodules --recurse-submodules WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies OUTPUT_QUIET)
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork -B ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build -DMSDF_ATLAS_INSTALL=ON -DMSDF_ATLAS_DYNAMIC_RUNTIME=ON OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build --config Debug OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build --config Release OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/install OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build --config Debug --prefix "${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/install" OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/build --config Release --prefix "${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/install" OUTPUT_QUIET)
set(msdf-atlas-gen_DIR ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/install/lib/cmake/msdf-atlas-gen)
set(msdfgen_DIR ${PROJECT_BINARY_DIR}/dependencies/msdf-atlas-gen-fork/install/lib/cmake/msdfgen)
find_package(msdf-atlas-gen REQUIRED)

message(STATUS "Preparing spdlog...")
if (NOT EXISTS "${PROJECT_BINARY_DIR}/dependencies/spdlog/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/gabime/spdlog.git WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies OUTPUT_QUIET)
    execute_process(COMMAND ${GIT_EXECUTABLE} checkout 3aceda0 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies/spdlog OUTPUT_QUIET)
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/spdlog/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${PROJECT_BINARY_DIR}/dependencies/spdlog -B ${PROJECT_BINARY_DIR}/dependencies/spdlog/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/spdlog/build --config Debug OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/spdlog/build --config Release OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/spdlog/install_dir OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/spdlog/build --config Debug --prefix "${PROJECT_BINARY_DIR}/dependencies/spdlog/install_dir" OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/spdlog/build --config Release --prefix "${PROJECT_BINARY_DIR}/dependencies/spdlog/install_dir" OUTPUT_QUIET)
set(spdlog_DIR ${PROJECT_BINARY_DIR}/dependencies/spdlog/install_dir/lib/cmake/spdlog)
find_package(spdlog REQUIRED)

message(STATUS "Preparing yaml-cpp...")
if (NOT EXISTS "${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/jbeder/yaml-cpp.git --depth 1 WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/dependencies OUTPUT_QUIET)
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -S ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp -B ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build --config Debug OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build --config Release OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/install OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build --config Debug --prefix "${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/install" OUTPUT_QUIET)
execute_process(COMMAND ${CMAKE_COMMAND} --install ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/build --config Release --prefix "${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/install" OUTPUT_QUIET)
set(yaml-cpp_DIR ${PROJECT_BINARY_DIR}/dependencies/yaml-cpp/install/lib/cmake/yaml-cpp)
find_package(yaml-cpp REQUIRED)
