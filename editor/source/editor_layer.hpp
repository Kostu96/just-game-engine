/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "windows/inspector.hpp"
#include "windows/scene_hierarchy.hpp"

#include <jng/core/base.hpp>
#include <jng/core/layer.hpp>
#include <jng/renderer/camera.hpp>
#include <jng/scene/entity.hpp>
#include <jng/scene/scene.hpp>

namespace jng {

    class Framebuffer;

    struct EditorContext
    {
        Scene activeScene;

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
        glm::vec2 m_viewportWindowSize{ 1.f, 1.f };
        Ref<Framebuffer> m_viewportFramebuffer;
        EditorContext m_context;

        InspectorWindow m_inspectorWindow;
        SceneHierarchyWindow m_sceneHierarchyWindow;
    };

} // namespace jng
