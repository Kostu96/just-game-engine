/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "widgets/inspector.hpp"
#include "widgets/main_menu_bar.hpp"
#include "widgets/scene_hierarchy.hpp"

#include <jng/core/base.hpp>
#include <jng/core/layer.hpp>
#include <jng/scene/camera.hpp>
#include <jng/scene/entity.hpp>
#include <jng/scene/scene.hpp>

namespace jng {

    class Framebuffer;

    struct EditorContext
    {
        glm::vec2 viewportWindowSize{ 1.f, 1.f };
        Ref<Scene> activeScene;
        Entity selectedEntity;

        bool isInspectorWindowOpen = true;
        bool isSceneHierarchyWindowOpen = true;
        bool isViewportWindowOpen = true;
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
    private:
        Ref<Framebuffer> m_viewportFramebuffer;
        EditorContext m_context;

        MainMenuBar m_mainMenuBar;
        InspectorWindow m_inspectorWindow;
        SceneHierarchyWindow m_sceneHierarchyWindow;
    };

} // namespace jng
