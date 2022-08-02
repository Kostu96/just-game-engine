/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scene_hierarchy.hpp"

#include "../editor_context.hpp"

#include <jng/scene/components.hpp>
#include <jng/scene/entity.hpp>

#include <imgui.h>

namespace jng {

	void SceneHierarchyWindow::onImGuiUpdate()
	{
        if (m_context.IsSceneHierarchyWindowOpen)
        {
            ImGui::SetNextWindowSize({ 320.f, 400.f }); // TODO: this is temporary to prevent window being too small when app is started first time
            ImGui::Begin("Scene Hierarchy", &m_context.IsSceneHierarchyWindowOpen, ImGuiWindowFlags_NoCollapse);

            if (m_context.ActiveScene)
            {
                m_context.ActiveScene->each([this](Entity entity) {
                    auto& tag = entity.getComponent<TagComponent>().Tag;

                    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth |
                        (m_context.SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);
                    flags |= ImGuiTreeNodeFlags_Leaf; // TODO: set unconditionaly until parenting logic
                    if (ImGui::TreeNodeEx(entity, flags, tag.c_str()))
                        ImGui::TreePop();

                    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                        m_context.SelectedEntity = entity;

                    if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonRight))
                    {
                        if (ImGui::MenuItem("Duplicate", "Ctrl+D"))
                        {
                            m_context.ActiveScene->duplicateEntity(entity);
                        }

                        if (ImGui::MenuItem("Delete", "Del"))
                        {
                            if (m_context.SelectedEntity == entity)
                                m_context.SelectedEntity = {};

                            m_context.ActiveScene->destroyEntity(entity);
                        }

                        ImGui::EndPopup();
                    }
                });

                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
                    m_context.SelectedEntity = {};

                if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
                {
                    // TODO: move this so it can be shared with menu on mainMenuBar
                    if (ImGui::BeginMenu("Create"))
                    {
                        if (ImGui::MenuItem("Empty Entity"))
                            m_context.ActiveScene->createEntity("Empty Entity");

                        ImGui::EndMenu();
                    }

                    ImGui::EndPopup();
                }
            }

            ImGui::End();
        }
	}

} // namespace jng
