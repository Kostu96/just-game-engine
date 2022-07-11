/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "editor_layer.hpp"

namespace jng {

    EditorLayer::EditorLayer(const Properties& properties) :
        m_viewportFramebuffer{ Framebuffer::create({ properties.width, properties.height }) },
        m_mainCamera{ -(properties.width / 20.f), properties.width / 20.f, -(properties.height / 20.f), properties.height / 20.f }
    {
        Entity entity1 = m_activeScene.createEntity();
        entity1.addComponent<SpriteComponent>();
    }

    void EditorLayer::onUpdate(float /*dt*/)
    {
        m_viewportFramebuffer->bind();
        jng::RendererAPI::clear({ .6f, 0.2f, .6f });
        Renderer2D::beginScene(m_mainCamera);

        m_activeScene.onUpdate();
        
        Renderer2D::endScene();
        m_viewportFramebuffer->unbind();

        updateGUI();
    }

    void EditorLayer::updateGUI()
    {
        // DockSpace
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;

        static bool dockspaceOpen = true;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpaceWindow", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();
        {
            ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

            // MenuBar
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Exit")) Engine::get().close();

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Edit"))
                {
                    ImGui::MenuItem("Undo");

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Windows"))
                {
                    ImGui::MenuItem("Inspector");
                    ImGui::MenuItem("Scene Hierarchy");
                    ImGui::MenuItem("Viewport");

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            // Scene
            ImGui::Begin("Scene Hierarchy");
            {
                m_activeScene.each([](Entity entity) {
                    auto& tc = entity.getComponent<TagComponent>();
                    ImGui::Text("%s", tc.tag.c_str());
                });
                ImGui::End();
            }

            // Viewport
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Viewport");
            ImGui::PopStyleVar();
            {
                ImVec2 viewportWindowSize = ImGui::GetContentRegionAvail();
                ImGui::Image(m_viewportFramebuffer->getColorAttachmentHandle(), { viewportWindowSize.x, viewportWindowSize.y });
                if (m_viewportWindowSize.x != viewportWindowSize.x || m_viewportWindowSize.y != viewportWindowSize.y)
                {
                    m_viewportWindowSize = { viewportWindowSize.x, viewportWindowSize.y };
                    m_viewportFramebuffer->resize(static_cast<uint32>(m_viewportWindowSize.x), static_cast<uint32>(m_viewportWindowSize.y));
                }

                ImGui::End();
            }

            // Inspector
            ImGui::Begin("Inspector");
            {

                ImGui::End();
            }

            ImGui::End();
        }
    }

}
