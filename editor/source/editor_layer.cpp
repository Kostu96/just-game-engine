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

    EditorLayer::EditorLayer(const Properties& /*properties*/) :
        m_viewportFramebuffer{ Framebuffer::create({
            .Width = 1, .Height = 1,
            .AttachmentsSpecifications = { jng::TextureFormat::RGBA8, jng::TextureFormat::Depth24Stencil8 }
        }) },
        m_mainMenuBar{ m_context },
        m_inspectorWindow{ m_context },
        m_sceneHierarchyWindow{ m_context },
        m_contentBrowserWindow{ m_context }
    {
    }

    void EditorLayer::onUpdate(float /*dt*/)
    {
        if (m_context.IsViewportWindowOpen)
        {
            if (m_context.ViewportWindowSize.x != m_viewportFramebuffer->getProperties().Width ||
                m_context.ViewportWindowSize.y != m_viewportFramebuffer->getProperties().Height) {
                uint32 newViewportWidth = static_cast<uint32>(m_context.ViewportWindowSize.x);
                uint32 newViewportHeight = static_cast<uint32>(m_context.ViewportWindowSize.y);
                m_viewportFramebuffer->resize(newViewportWidth, newViewportHeight);
                m_editorCamera.setViewportSize(newViewportWidth, newViewportHeight);
            }

            if (m_context.IsViewportWindowFocused)
                m_editorCamera.onUpdate();

            m_viewportFramebuffer->bind();
            jng::RendererAPI::clear({ 0.1f, 0.15f, 0.2f });
            if (m_context.ActiveScene)
            {
                jng::Renderer2D::beginScene(m_editorCamera.getVP());
                m_context.ActiveScene->drawSprites();
                jng::Renderer2D::endScene();
            }
            m_viewportFramebuffer->unbind();
        }
    }

    void EditorLayer::onImGuiUpdate()
    {
        // DockSpace
        auto& style = ImGui::GetStyle();
        ImVec2 windowMinSize = style.WindowMinSize;
        style.WindowMinSize.x = 320.f;
        style.WindowMinSize.y = 250.f;
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        style.WindowMinSize = windowMinSize;

        m_mainMenuBar.onImGuiUpdate();

        if (m_context.IsProjectOpen)
        {
            // Viewport
            if (m_context.IsViewportWindowOpen)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::SetNextWindowSize({ 160 * 4.f, 90 * 4.f }); // TODO: this is temporary to prevent window being too small when app is started first time
                ImGui::Begin("Viewport", &m_context.IsViewportWindowOpen, ImGuiWindowFlags_NoCollapse);
                ImGui::PopStyleVar();
                m_context.IsViewportWindowFocused = ImGui::IsWindowFocused();
                m_context.ViewportWindowSize = ImGui::GetContentRegionAvail();
                ImGui::Image(m_viewportFramebuffer->getColorAttachmentHandle(), m_context.ViewportWindowSize);

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                    {
                        const char* path = reinterpret_cast<const char*>(payload->Data);
                        m_context.openScene(path);
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::End();
            }

            m_inspectorWindow.onImGuiUpdate();
            m_sceneHierarchyWindow.onImGuiUpdate();
            m_contentBrowserWindow.onImGuiUpdate();
        }
    }

    void EditorLayer::onEvent(Event& event)
    {
        if (m_context.IsViewportWindowFocused)
            m_editorCamera.onEvent(event);
    }

}
