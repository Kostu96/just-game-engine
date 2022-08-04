/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "editor_context.hpp"
#include "widgets/content_browser.hpp"
#include "widgets/inspector.hpp"
#include "widgets/main_menu_bar.hpp"
#include "widgets/scene_hierarchy.hpp"
#include "widgets/statistics.hpp"

#include <jng/core/base.hpp>
#include <jng/core/layer.hpp>
#include <jng/scene/camera.hpp>
#include <jng/scene/entity.hpp>
#include <jng/scene/scene.hpp>

#include <filesystem>

namespace jng {

    class Framebuffer;
    class KeyPressEvent;
    class MouseButtonPressEvent;

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
        bool onKeyPress(KeyPressEvent& event);
        bool onMouseButtonPress(MouseButtonPressEvent& event);
        
        Ref<Framebuffer> m_viewportFramebuffer;

        EditorContext m_context;
        int m_gizmoType = -1;

        MainMenuBar m_mainMenuBar;
        InspectorWindow m_inspectorWindow;
        SceneHierarchyWindow m_sceneHierarchyWindow;
        ContentBrowserWindow m_contentBrowserWindow;
        StatisticsWindow m_statisticsWindow;

        Ref<Texture> m_playIcon, m_stopIcon;
    };

} // namespace jng
