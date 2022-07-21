/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "editor_camera.hpp"
#include "widgets/content_browser.hpp"
#include "widgets/inspector.hpp"
#include "widgets/main_menu_bar.hpp"
#include "widgets/scene_hierarchy.hpp"

#include <jng/core/base.hpp>
#include <jng/core/layer.hpp>
#include <jng/scene/camera.hpp>
#include <jng/scene/entity.hpp>
#include <jng/scene/scene.hpp>

#include <filesystem>

namespace jng {

    class Framebuffer;

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
    };

    class EditorLayer :
        public Layer
    {
    public:
        struct Properties
        {
            uint32 width;
            uint32 height;
        };

        explicit EditorLayer(const Properties& properties);

        void onUpdate(float dt) override;
        void onImGuiUpdate() override;
        void onEvent(Event& event) override;
    private:
        Ref<Framebuffer> m_viewportFramebuffer;
        EditorCamera m_editorCamera;
        EditorContext m_context;

        MainMenuBar m_mainMenuBar;
        InspectorWindow m_inspectorWindow;
        SceneHierarchyWindow m_sceneHierarchyWindow;
        ContentBrowserWindow m_contentBrowserWindow;
    };

} // namespace jng
