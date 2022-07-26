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
#include <imgui_internal.h>

namespace jng {

    EditorLayer::EditorLayer(const Properties& /*properties*/) :
        m_viewportFramebuffer{ Framebuffer::create({
            .Width = 1, .Height = 1,
            .AttachmentsSpecifications = { jng::TextureFormat::RGBA8, jng::TextureFormat::Depth24Stencil8 }
        }) },
        m_mainMenuBar{ m_context },
        m_inspectorWindow{ m_context },
        m_sceneHierarchyWindow{ m_context },
        m_contentBrowserWindow{ m_context },
        m_playIcon{ Texture::create("assets/textures/play_icon.png") },
        m_stopIcon{ Texture::create("assets/textures/stop_icon.png") }
    {
    }

    void EditorLayer::onUpdate(float dt)
    {
        if (m_context.IsViewportWindowOpen)
        {
            if (m_context.ViewportWindowSize.x != m_viewportFramebuffer->getProperties().Width ||
                m_context.ViewportWindowSize.y != m_viewportFramebuffer->getProperties().Height)
            {
                m_viewportFramebuffer->resize(
                    static_cast<uint32>(m_context.ViewportWindowSize.x),
                    static_cast<uint32>(m_context.ViewportWindowSize.y)
                );
                m_editorCamera.setViewportSize(m_context.ViewportWindowSize.x, m_context.ViewportWindowSize.y);

                if (m_context.ActiveScene)
                    m_context.ActiveScene->setViewportSize(m_context.ViewportWindowSize.x, m_context.ViewportWindowSize.y);
            }

            if (m_context.IsViewportWindowFocused)
                m_editorCamera.onUpdate();

            m_viewportFramebuffer->bind();
            jng::RendererAPI::clear({ 0.1f, 0.15f, 0.2f });
            if (m_context.ActiveScene)
            {
                switch (m_context.SceneState)
                {
                case SceneState::Stopped:
                    jng::Renderer2D::beginScene(m_editorCamera.getVP());
                    m_context.ActiveScene->drawSprites();
                    jng::Renderer2D::endScene();
                    break;
                case SceneState::Playing:
                    m_context.ActiveScene->onUpdate(dt);
                    break;
                }
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
        style.WindowMinSize.y = 48.f;
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        style.WindowMinSize = windowMinSize;

        m_mainMenuBar.onImGuiUpdate();

        if (m_context.IsProjectOpen)
        {
            // Viewport
            if (m_context.IsViewportWindowOpen)
            {
                ImGuiWindowClass toolbarClass;
                toolbarClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoResizeY | ImGuiDockNodeFlags_NoTabBar;
                ImGui::SetNextWindowClass(&toolbarClass);
                ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
                Ref<Texture> icon = m_context.SceneState == SceneState::Stopped ? m_playIcon : m_stopIcon;
                ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.3f, 0.6f, 0.5f });
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.f, 0.f, 0.f, 0.f });
                ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x - 32.f) * 0.5f);
                if (ImGui::ImageButton(icon->getRendererID(), { 32.f, 32.f }))
                {
                    switch (m_context.SceneState)
                    {
                    case SceneState::Stopped:
                        m_context.SceneState = SceneState::Playing;
                        m_context.ActiveScene->onCreate();
                        break;
                    case SceneState::Playing:
                        m_context.SceneState = SceneState::Stopped;
                        m_context.ActiveScene->onDestroy();
                        break;
                    }
                }
                ImGui::PopStyleColor(3);
                ImGui::End();

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
