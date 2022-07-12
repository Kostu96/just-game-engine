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

				if (m_context.selectedEntity.hasComponent<CameraComponent>())
				{
					if (ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(CameraComponent).hash_code()), componentTreeNodeFlags, "Camera"))
					{
						auto& cc = m_context.selectedEntity.getComponent<CameraComponent>();

						static const char* projectionTypes[] = { "Orthographic", "Perspective" };
						uint32 selectedIndex = static_cast<uint32>(cc.projectionType);
						if (ImGui::BeginCombo("Projection", projectionTypes[selectedIndex]))
						{
							if (ImGui::Selectable(projectionTypes[0], selectedIndex == 0))
								cc.projectionType = CameraComponent::ProjectionType::Orthographic;

							if (ImGui::Selectable(projectionTypes[1], selectedIndex == 1))
								cc.projectionType = CameraComponent::ProjectionType::Perspective;

							ImGui::EndCombo();
						}

						switch (cc.projectionType)
						{
						case CameraComponent::ProjectionType::Orthographic:
							ImGui::DragFloat("Size", &cc.orthoSize, 0.1f);
							ImGui::DragFloat("Near", &cc.orthoNear, 0.1f);
							ImGui::DragFloat("Far", &cc.orthoFar, 0.1f);
							break;
						case CameraComponent::ProjectionType::Perspective:
							ImGui::DragFloat("FOV", &cc.perspectiveFOV, 0.1f);
							ImGui::DragFloat("Near", &cc.perspectiveNear, 0.1f);
							ImGui::DragFloat("Far", &cc.perspectiveFar, 0.1f);
							break;
						}

						ImGui::TreePop();
					}


				}

				if (m_context.selectedEntity.hasComponent<SpriteComponent>())
				{
					if (ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(SpriteComponent).hash_code()), componentTreeNodeFlags, "Sprite"))
					{
						auto& sc = m_context.selectedEntity.getComponent<SpriteComponent>();

						ImGui::ColorEdit4("Color", glm::value_ptr(sc.color));

						ImGui::TreePop();
					}


				}
			}

			ImGui::End();
		}
	}

} // namespace jng
