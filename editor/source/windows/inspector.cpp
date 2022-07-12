/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "inspector.hpp"

#include "../editor_layer.hpp"

#include <jng/scene/components.hpp>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace jng {

	void InspectorWindow::onImGuiUpdate()
	{
		if (m_context.isInspectorWindowOpen)
		{
			static ImGuiTreeNodeFlags componentTreeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen;

			ImGui::Begin("Inspector", &m_context.isInspectorWindowOpen, ImGuiWindowFlags_NoCollapse);

			if (m_context.selectedEntity)
			{
				if (m_context.selectedEntity.hasComponent<TagComponent>())
				{
					auto& tag = m_context.selectedEntity.getComponent<TagComponent>().tag;

					char buffer[128];
					strcpy_s(buffer, sizeof(buffer), tag.c_str());
					if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
						tag = buffer;
				}

				if (m_context.selectedEntity.hasComponent<TransformComponent>())
				{
					if (ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(TransformComponent).hash_code()), componentTreeNodeFlags, "Transform"))
					{
						auto& tc = m_context.selectedEntity.getComponent<TransformComponent>();

						ImGui::DragFloat3("Translation", glm::value_ptr(tc.translation), 0.1f);
						ImGui::DragFloat3("Rotation", glm::value_ptr(tc.rotation), 0.1f);
						ImGui::DragFloat3("Scale", glm::value_ptr(tc.scale), 0.1f);

						ImGui::TreePop();
					}

					
				}
			}

			ImGui::End();
		}
	}

} // namespace jng
