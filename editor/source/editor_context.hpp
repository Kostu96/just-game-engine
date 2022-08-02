/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "editor_camera.hpp"

#include <jng/core/base.hpp>
#include <jng/scene/entity.hpp>

#include <glm/glm.hpp>
#include <filesystem>

namespace jng {
    
    class Scene;

    enum class SceneState
    {
        Stopped,
        Playing
    };

    struct EditorContext
    {
        bool IsProjectOpen = false;
        std::filesystem::path ProjectPath;
        std::filesystem::path AssetsPath;
        Ref<Scene> ActiveScene;
        SceneState SceneState = SceneState::Stopped;

        // Viewport state:
        EditorCamera EditorCamera;
        glm::vec2 ViewportWindowSize{ 1.f, 1.f };
        glm::vec2 MousePosWithinViewport;
        bool IsViewportWindowOpen = true;
        bool IsViewportWindowActive = false;

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
