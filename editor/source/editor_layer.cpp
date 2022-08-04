/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "editor_layer.hpp"

#include <jng/core/event.hpp>
#include <jng/core/engine.hpp>
#include <jng/core/key_events.hpp>
#include <jng/core/mouse_events.hpp>
#include <jng/platform/input.hpp>
#include <jng/renderer/framebuffer.hpp>
#include <jng/renderer/renderer2d.hpp>
#include <jng/renderer/renderer_api.hpp>
#include <jng/scene/components.hpp>
#include <jng/scene/entity.hpp>
#include <jng/utilities/math.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

namespace jng {

    EditorLayer::EditorLayer(const Properties& /*properties*/) :
        m_viewportFramebuffer{ Framebuffer::create({
            .Width = 1, .Height = 1,
            .AttachmentsSpecifications = {
                jng::TextureFormat::RGBA8,
                jng::TextureFormat::R32,
                jng::TextureFormat::Depth24Stencil8
            }
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
        if (m_context.IsViewportWindowOpen && m_context.IsProjectOpen)
        {
            if (m_context.ViewportWindowSize.x != m_viewportFramebuffer->getProperties().Width ||
                m_context.ViewportWindowSize.y != m_viewportFramebuffer->getProperties().Height)
            {
                m_viewportFramebuffer->resize(
                    static_cast<uint32>(m_context.ViewportWindowSize.x),
                    static_cast<uint32>(m_context.ViewportWindowSize.y)
                );
                m_context.EditorCamera.setViewportSize(m_context.ViewportWindowSize.x, m_context.ViewportWindowSize.y);

                if (m_context.ActiveScene)
                    m_context.ActiveScene->setViewportSize(m_context.ViewportWindowSize.x, m_context.ViewportWindowSize.y);
            }

            if (m_context.IsViewportWindowActive)
                m_context.EditorCamera.onUpdate();

            m_viewportFramebuffer->bind();
            jng::RendererAPI::clear({ 0.1f, 0.15f, 0.2f });
            m_viewportFramebuffer->clearAttachment(1, -1);

            if (m_context.ActiveScene)
            {
                switch (m_context.SceneState)
                {
                case SceneState::Stopped:
                {
                    jng::Renderer2D::beginScene(m_context.EditorCamera.getViewProjection());
                    m_context.ActiveScene->drawRenderables();

                    // Draw colliders
                    auto& registry = m_context.ActiveScene->m_registry;
                    {
                        auto group = registry.group<BoxCollider2DComponent>(entt::get<TransformComponent>);
                        for (auto entity : group)
                        {
                            auto [bcc, tc] = group.get<BoxCollider2DComponent, TransformComponent>(entity);
                            glm::vec3 translation = tc.Translation;
                            glm::vec3 scale = tc.Scale;
                            glm::mat4 transform = glm::translate(glm::mat4{ 1.f }, translation) * glm::scale(glm::mat4{ 1.f }, scale);
                            Renderer2D::drawRect(transform, { 0.25f, 1.f, 0.f, 1.f });
                        }
                    }
                    {
                        auto group = registry.group<CircleCollider2DComponent>(entt::get<TransformComponent>);
                        for (auto entity : group)
                        {
                            auto [ccc, tc] = group.get<CircleCollider2DComponent, TransformComponent>(entity);
                            glm::vec3 translation = tc.Translation + glm::vec3{ ccc.offset, 0.001f };
                            glm::vec3 scale = tc.Scale * ccc.radius * 2.f;
                            glm::mat4 transform = glm::translate(glm::mat4{ 1.f }, translation) * glm::scale(glm::mat4{ 1.f }, scale);
                            Renderer2D::drawCircle(transform, { 0.25f, 1.f, 0.f, 1.f }, 0.05f);
                        }
                    }

                    jng::Renderer2D::endScene();
                }   break;
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
        ImGuizmo::BeginFrame();

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
                    case SceneState::Stopped: m_context.onSceneStart(); break;
                    case SceneState::Playing: m_context.onSceneStop();  break;
                    }
                }
                ImGui::PopStyleColor(3);
                ImGui::End();

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::SetNextWindowSize({ 160 * 4.f, 90 * 4.f }); // TODO: this is temporary to prevent window being too small when app is started first time
                ImGui::Begin("Viewport", &m_context.IsViewportWindowOpen, ImGuiWindowFlags_NoCollapse);
                ImGui::PopStyleVar();

                m_context.IsViewportWindowActive = ImGui::IsWindowFocused() || ImGui::IsWindowHovered();

                glm::vec2 viewportOffset = ImGui::GetCursorPos();
                glm::vec2 mousePos = ImGui::GetMousePos();
                glm::vec2 viewportPos = ImGui::GetWindowPos();

                m_context.ViewportWindowSize = ImGui::GetContentRegionAvail();
                ImGui::Image(m_viewportFramebuffer->getColorAttachments()[0]->getRendererID(), m_context.ViewportWindowSize);

                m_context.MousePosWithinViewport = mousePos - viewportPos - viewportOffset;
                if (m_context.MousePosWithinViewport.x > m_context.ViewportWindowSize.x)
                    m_context.MousePosWithinViewport.x = -1.f;
                if (m_context.MousePosWithinViewport.y > m_context.ViewportWindowSize.y)
                    m_context.MousePosWithinViewport.y = -1.f;

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                    {
                        const char* path = reinterpret_cast<const char*>(payload->Data);
                        m_context.openScene(path);
                    }
                    ImGui::EndDragDropTarget();
                }

                // Gizmos
                if (m_context.SelectedEntity && m_gizmoType != -1)
                {
                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();
                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                                      ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

                    glm::mat4 cameraView = m_context.EditorCamera.getView();
                    const glm::mat4& cameraProjection = m_context.EditorCamera.getProjection();
                    auto& tc = m_context.SelectedEntity.getComponent<TransformComponent>();
                    glm::mat4 transform = tc.getTransform();

                    bool snap = Input::isKeyPressed(Key::LeftControl);
                    float snapValue = m_gizmoType == ImGuizmo::OPERATION::ROTATE ? 15.f : 0.5f;

                    ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                        static_cast<ImGuizmo::OPERATION>(m_gizmoType), ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),
                        nullptr, snap ? &snapValue : nullptr);

                    if (ImGuizmo::IsUsing())
                    {
                        glm::vec3 rotation;
                        math::decomposeTransform(transform, tc.Translation, rotation, tc.Scale);
                        glm::vec3 rotDelta = rotation - tc.Rotation;
                        tc.Rotation += rotDelta;
                    }
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
        if (m_context.IsViewportWindowActive)
            m_context.EditorCamera.onEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.dispatch<KeyPressEvent>(JNG_BIND_EVENT_FUNC(EditorLayer::onKeyPress));
        dispatcher.dispatch<MouseButtonPressEvent>(JNG_BIND_EVENT_FUNC(EditorLayer::onMouseButtonPress));
    }

    bool EditorLayer::onKeyPress(KeyPressEvent& event)
    {
        switch (event.getKeyCode())
        {
        case Key::Q:
            m_gizmoType = -1;
            break;
        case Key::W:
            m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case Key::E:
            m_gizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        case Key::R:
            m_gizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        }

        return false;
    }

    bool EditorLayer::onMouseButtonPress(MouseButtonPressEvent& event)
    {
        if (event.getMouseButton() == Mouse::Left && !ImGuizmo::IsOver() &&
            m_context.MousePosWithinViewport.x > 0.f && m_context.MousePosWithinViewport.y > 0.f)
        {
            m_viewportFramebuffer->bind();
            int pixel = m_viewportFramebuffer->readPixel(1,
                static_cast<uint32>(m_context.MousePosWithinViewport.x),
                static_cast<uint32>(m_context.MousePosWithinViewport.y)
            );
            m_viewportFramebuffer->unbind();
            m_context.SelectedEntity = pixel >= 0 ? Entity{ static_cast<entt::entity>(pixel), *m_context.ActiveScene.get() } : Entity{};
        }

        return false;
    }

}
