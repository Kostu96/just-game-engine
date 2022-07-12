/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "editor_layer.hpp"

#include <jng/core/engine.hpp>
#include <jng/scene/components.hpp>
#include <jng/scene/entity.hpp>
#include <jng/renderer/framebuffer.hpp>
#include <jng/renderer/renderer2d.hpp>
#include <jng/renderer/renderer_api.hpp>

#include <imgui.h>

namespace jng {

    EditorLayer::EditorLayer(const Properties& properties) :
        m_viewportFramebuffer{ Framebuffer::create({ 1, 1 }) },
        m_mainCamera{ -(properties.width / 50.f), properties.width / 50.f, -(properties.height / 50.f), properties.height / 50.f },
        m_inspectorWindow{ m_context },
        m_sceneHierarchyWindow{ m_context }
    {
        Entity entity1 = m_context.activeScene.createEntity("Square");
        entity1.addComponent<SpriteComponent>();
    }

    void EditorLayer::onUpdate(float /*dt*/)
    {
        if (m_viewportWindowSize.x != m_viewportFramebuffer->getProperties().width || m_viewportWindowSize.y != m_viewportFramebuffer->getProperties().height)
            m_viewportFramebuffer->resize(static_cast<uint32>(m_viewportWindowSize.x), static_cast<uint32>(m_viewportWindowSize.y));

        m_viewportFramebuffer->bind();
        jng::RendererAPI::clear({ .6f, 0.2f, .6f });
        Renderer2D::beginScene(m_mainCamera);

        m_context.activeScene.onUpdate();
        
        Renderer2D::endScene();
        m_viewportFramebuffer->unbind();
    }

    void EditorLayer::onImGuiUpdate()
    {
        // DockSpace
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        // MainMenuBar
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("New Project", nullptr, nullptr, false);
                ImGui::MenuItem("Save", "Ctrl + S", nullptr, false);
                ImGui::MenuItem("Save As", nullptr, nullptr, false);
                if (ImGui::MenuItem("Exit")) Engine::get().close();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::MenuItem("Undo", "Ctrl + Z");

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Inspector", nullptr, &m_context.isInspectorWindowOpen);
                ImGui::MenuItem("Scene Hierarchy", nullptr, &m_context.isSceneHierarchyWindowOpen);
                ImGui::MenuItem("Viewport", nullptr, &m_context.isViewportWindowOpen);

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Viewport
        if (m_context.isViewportWindowOpen)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Viewport", &m_context.isViewportWindowOpen, ImGuiWindowFlags_NoCollapse);
            ImGui::PopStyleVar();
            auto viewportWindowSize = ImGui::GetContentRegionAvail();
            m_viewportWindowSize.x = viewportWindowSize.x;
            m_viewportWindowSize.y = viewportWindowSize.y;
            ImGui::Image(m_viewportFramebuffer->getColorAttachmentHandle(), { m_viewportWindowSize.x, m_viewportWindowSize.y });
            ImGui::End();
        }
  
        m_inspectorWindow.onImGuiUpdate();
        m_sceneHierarchyWindow.onImGuiUpdate();
    }

}