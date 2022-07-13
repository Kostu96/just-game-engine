/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scene_hierarchy.hpp"

#include "../editor_layer.hpp"

#include <jng/scene/components.hpp>
#include <jng/scene/entity.hpp>

#include <imgui.h>

namespace jng {

	void SceneHierarchyWindow::onImGuiUpdate()
	{
        if (m_context.isSceneHierarchyWindowOpen)
        {
            ImGui::SetNextWindowSize({ 320.f, 400.f }); // TODO: temp
            ImGui::Begin("Scene Hierarchy", &m_context.isSceneHierarchyWindowOpen, ImGuiWindowFlags_NoCollapse);

            m_context.activeScene.each([this](Entity entity) {
                auto& tag = entity.getComponent<TagComponent>().tag;

                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth |
                    (m_context.selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);
                if (ImGui::TreeNodeEx(entity, flags, tag.c_str()))
                    ImGui::TreePop();

                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                    m_context.selectedEntity = entity;

                if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonRight))
                {
                    if (ImGui::MenuItem("Delete")) {
                        if (m_context.selectedEntity == entity)
                            m_context.selectedEntity = {};

                        m_context.activeScene.destroyEntity(entity);
                    }

                    ImGui::EndPopup();
                }

                });

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
                m_context.selectedEntity = {};

            if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
            {
                // TODO: move this so it can be shared with menu on mainMenuBar
                if (ImGui::BeginMenu("Create"))
                {
                    if (ImGui::MenuItem("Empty Entity"))
                        m_context.activeScene.createEntity("Empty Entity");

                    ImGui::EndMenu();
                }

                ImGui::EndPopup();
            }

            ImGui::End();
        }
	}

} // namespace jng
