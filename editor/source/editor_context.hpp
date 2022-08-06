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
        std::filesystem::path EditorScenePath;
        Ref<Scene> EditorScene;
        Ref<Scene> ActiveScene;
        SceneState SceneState = SceneState::Stopped;

        // Viewport state:
        EditorCamera EditorCamera;
        glm::vec2 ViewportWindowSize{ 1.f, 1.f };
        glm::vec2 MousePosWithinViewport{};
        bool IsViewportWindowOpen = true;
        bool IsViewportWindowActive = false;

        // Scene Hierachy and Inspector state:
        Entity SelectedEntity;
        bool IsSceneHierarchyWindowOpen = true;
        bool IsInspectorWindowOpen = true;

        // Content Browser state:
        std::filesystem::path BrowsedPath;
        bool IsContentBrowserWindowOpen = true;

        // Statistics state:
        bool isStatisticsWindowOpen = true;

        // Settings state:
        bool isSettingsWindowOpen = true;
        float physicsGravity = 9.8f;
        bool showColliders = false;

        void createProject(std::filesystem::path path);
        void openProject(std::filesystem::path path);

        void createScene();
        void openScene(std::filesystem::path path);
        void saveScene(std::filesystem::path path);

        void onSceneStart();
        void onSceneStop();
    };

} // namespace jng
