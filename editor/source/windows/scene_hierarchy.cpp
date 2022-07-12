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

            m_context.activeScene.each([](Entity entity) {
                auto& tc = entity.getComponent<TagComponent>();

                if (ImGui::TreeNodeEx(tc.tag.c_str(), ImGuiTreeNodeFlags_OpenOnArrow))
                    ImGui::TreePop();

                });

            ImGui::End();
        }
	}

} // namespace jng
