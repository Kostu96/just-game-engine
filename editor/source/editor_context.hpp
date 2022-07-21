/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/core/base.hpp>
#include <jng/scene/entity.hpp>

#include <glm/glm.hpp>
#include <filesystem>

namespace jng {
    
    class Scene;

    struct EditorContext
    {
        bool IsProjectOpen = false;
        std::filesystem::path ProjectPath;
        std::filesystem::path AssetsPath;
        Ref<Scene> ActiveScene;

        // Viewport state:
        glm::vec2 ViewportWindowSize{ 1.f, 1.f };
        bool IsViewportWindowOpen = true;
        bool IsViewportWindowFocused = false;

        // Scene Hierachy and Inspector state:
        Entity SelectedEntity;
        bool IsSceneHierarchyWindowOpen = true;
        bool IsInspectorWindowOpen = true;

        // Content Browser state:
        std::filesystem::path BrowsedPath;
        bool IsContentBrowserWindowOpen = true;

        void openScene(std::filesystem::path path);
    };

} // namespace jng
