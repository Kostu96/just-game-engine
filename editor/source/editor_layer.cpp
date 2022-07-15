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
        m_viewportFramebuffer{ Framebuffer::create({ 1, 1 }) },
        m_mainMenuBar{ m_context },
        m_inspectorWindow{ m_context },
        m_sceneHierarchyWindow{ m_context }
    {
        m_context.activeScene = makeRef<Scene>();

        Entity camera = m_context.activeScene->createEntity("Main Camera");
        camera.addComponent<CameraComponent>();

        Entity square1 = m_context.activeScene->createEntity("Green Square");
        square1.getComponent<TransformComponent>().translation.x = -0.2f;
        square1.addComponent<SpriteComponent>().color = { 0.f, 1.f, 0.f, 1.f };

        Entity square2 = m_context.activeScene->createEntity("Red Square");
        square2.getComponent<TransformComponent>().translation.y = 0.2f;;
        square2.addComponent<SpriteComponent>().color = { 1.f, 0.f, 0.f, 1.f };
    }

    void EditorLayer::onUpdate(float /*dt*/)
    {
        if (m_context.viewportWindowSize.x != m_viewportFramebuffer->getProperties().width || m_context.viewportWindowSize.y != m_viewportFramebuffer->getProperties().height) {
            uint32 newViewportWidth = static_cast<uint32>(m_context.viewportWindowSize.x);
            uint32 newViewportHeight = static_cast<uint32>(m_context.viewportWindowSize.y);
            m_viewportFramebuffer->resize(newViewportWidth, newViewportHeight);
            
            // TODO: resize editor camera

            Camera* activeCamera = m_context.activeScene->getActiveCamera();
            if (activeCamera)
                activeCamera->setViewportSize(newViewportWidth, newViewportHeight);
        }

        m_viewportFramebuffer->bind();
        jng::RendererAPI::clear({ 0.1f, 0.15f, 0.2f });

        m_context.activeScene->onUpdate();
        
        m_viewportFramebuffer->unbind();
    }

    void EditorLayer::onImGuiUpdate()
    {
        // DockSpace
        auto& style = ImGui::GetStyle();
        float windowMinSize = style.WindowMinSize.x;
        style.WindowMinSize.x = 320.f;
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        style.WindowMinSize.x = windowMinSize;

        m_mainMenuBar.onImGuiUpdate();

        // Viewport
        if (m_context.isViewportWindowOpen)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize({ 160 * 4.f, 90 * 4.f }); // TODO: this is temporary to prevent window being too small when app is started first time
            ImGui::Begin("Viewport", &m_context.isViewportWindowOpen, ImGuiWindowFlags_NoCollapse);
            ImGui::PopStyleVar();
            m_context.viewportWindowSize = ImGui::GetContentRegionAvail();
            ImGui::Image(m_viewportFramebuffer->getColorAttachmentHandle(), m_context.viewportWindowSize);
            ImGui::End();
        }
  
        m_inspectorWindow.onImGuiUpdate();
        m_sceneHierarchyWindow.onImGuiUpdate();
    }

}
