#
# Copyright (C) 2022 Konstanty Misiak
#
# SPDX-License-Identifier: MIT
#

list(APPEND EDITOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/content_browser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/content_browser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inspector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inspector.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main_menu_bar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main_menu_bar.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene_hierarchy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene_hierarchy.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statistics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statistics.hpp
)

set(EDITOR_SOURCES ${EDITOR_SOURCES} PARENT_SCOPE)
