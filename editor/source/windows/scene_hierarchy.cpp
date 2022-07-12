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
            ImGui::Begin("Scene Hierarchy", &m_context.isSceneHierarchyWindowOpen, ImGuiWindowFlags_NoCollapse);

            m_context.activeScene.each([this](Entity entity) {
                auto& tag = entity.getComponent<TagComponent>().tag;

                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                    (m_context.selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);
                if (ImGui::TreeNodeEx(entity, flags, tag.c_str()))
                    ImGui::TreePop();

                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                    m_context.selectedEntity = entity;

                });

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
                m_context.selectedEntity = {};

            ImGui::End();
        }
	}

} // namespace jng
