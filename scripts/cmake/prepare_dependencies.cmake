#
# Copyright (C) 2023 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

find_package(Git QUIET REQUIRED)

function(prapare_dependency_from_git NAME URL SHA)
    message(STATUS "Preparing ${NAME}...")
    if (NOT EXISTS "dependencies/${NAME}/.git")
        execute_process(COMMAND ${GIT_EXECUTABLE} clone --shallow-submodules --recurse-submodules ${URL} ${NAME} WORKING_DIRECTORY "dependencies")
        execute_process(COMMAND ${GIT_EXECUTABLE} checkout ${SHA} WORKING_DIRECTORY "dependencies/${NAME}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "dependencies/${NAME}/build")
    execute_process(COMMAND ${CMAKE_COMMAND} -S "dependencies/${NAME}" -B "dependencies/${NAME}/build" ${ARGN})
    execute_process(COMMAND ${CMAKE_COMMAND} --build "dependencies/${NAME}/build" --config Debug)
    execute_process(COMMAND ${CMAKE_COMMAND} --build "dependencies/${NAME}/build" --config Release)
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "dependencies/${NAME}/build/install")
    execute_process(COMMAND ${CMAKE_COMMAND} --install "dependencies/${NAME}/build" --config Debug --prefix "dependencies/${NAME}/build/install")
    execute_process(COMMAND ${CMAKE_COMMAND} --install "dependencies/${NAME}/build" --config Release --prefix "dependencies/${NAME}/build/install")
endfunction()

function(prapare_dependency_from_ws NAME PATH)
    message(STATUS "Preparing ${NAME}...")
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "dependencies/${NAME}/build")
    execute_process(COMMAND ${CMAKE_COMMAND} -S ${PATH} -B "dependencies/${NAME}/build")
    execute_process(COMMAND ${CMAKE_COMMAND} --build "dependencies/${NAME}/build" --config Debug)
    execute_process(COMMAND ${CMAKE_COMMAND} --build "dependencies/${NAME}/build" --config Release)
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "dependencies/${NAME}/build/install")
    execute_process(COMMAND ${CMAKE_COMMAND} --install "dependencies/${NAME}/build" --config Debug --prefix "dependencies/${NAME}/build/install")
    execute_process(COMMAND ${CMAKE_COMMAND} --install "dependencies/${NAME}/build" --config Release --prefix "dependencies/${NAME}/build/install")
endfunction()

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "dependencies")

prapare_dependency_from_git(box2d          https://github.com/Kostu96/box2d-fork.git          8c6a8a1 -DBOX2D_BUILD_UNIT_TESTS=OFF -DBOX2D_BUILD_TESTBED=OFF -DBOX2D_BUILD_DOCS=OFF)
prapare_dependency_from_git(freetype       https://github.com/freetype/freetype.git           7d45cf2)
prapare_dependency_from_git(glfw3          https://github.com/glfw/glfw.git                   3eaf125 -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_DEBUG_POSTFIX=d)
prapare_dependency_from_git(glm            https://github.com/g-truc/glm.git                  47585fd -DBUILD_TESTING=OFF)
prapare_dependency_from_git(Lua            https://github.com/Kostu96/lua-5.4.4.git           9ebf101)
prapare_dependency_from_git(msdf-atlas-gen https://github.com/Kostu96/msdf-atlas-gen-fork.git 2815d0e -DMSDF_ATLAS_INSTALL=ON -DMSDF_ATLAS_DYNAMIC_RUNTIME=ON)
prapare_dependency_from_git(spdlog         https://github.com/gabime/spdlog.git               3aceda0 -DSPDLOG_BUILD_EXAMPLE=OFF)
prapare_dependency_from_git(yaml-cpp       https://github.com/jbeder/yaml-cpp.git             016b2e7)
prapare_dependency_from_ws( glad "engine/third_party/glad")
prapare_dependency_from_ws( stb  "engine/third_party/stb")
