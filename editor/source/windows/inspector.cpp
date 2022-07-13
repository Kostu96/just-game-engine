/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "inspector.hpp"

#include "../editor_layer.hpp"

#include <jng/core/base.hpp>
#include <jng/scene/components.hpp>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace jng {

	// TODO: move to shared place (imgui helpers?) so it can be reused
	/*static void updateImGuiVec3(const std::string& label, glm::vec3& values)
	{
		ImGui::BeginTable(label.c_str(), 2);
		ImGui::TableNextColumn();
		ImGui::Text(label.c_str());
		ImGui::TableNextColumn();
		ImGui::DragFloat3("##", glm::value_ptr(values), 0.1f, 0.f, 0.f, "%.2f");
		ImGui::EndTable();
	}*/

	// TODO: check Function type traits
	template<typename Component, typename Function>
	static void updateComponent(const char* label, Entity entity, Function function, bool isRemovable = true)
	{
		if (entity.hasComponent<Component>())
		{
			static ImGuiTreeNodeFlags componentTreeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4.f, 4.f });
			bool open = ImGui::CollapsingHeader(label, componentTreeNodeFlags);
			ImGui::PopStyleVar();

			bool markForDelete = false;
			if (isRemovable)
			{
				std::string str_id{ label };
				str_id += "ComponenetsSettings";

				ImGui::SameLine(contentRegionAvailable.x - 17.f);
				if (ImGui::Button("...", { 28.f, 28.f }))
					ImGui::OpenPopup(str_id.c_str());

				if (ImGui::BeginPopup(str_id.c_str())) {
					if (ImGui::MenuItem("Delete")) {
						markForDelete = true;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			if (open)
			{
				auto& component = entity.getComponent<Component>();
				function(component);

				//ImGui::TreePop();
			}

			if (markForDelete)
				entity.removeComponent<Component>();

			ImGui::Separator();
		}
	}

	void InspectorWindow::onImGuiUpdate()
	{
		if (m_context.isInspectorWindowOpen)
		{
			static ImGuiTreeNodeFlags componentTreeNodeFlags =
				ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			ImGui::SetNextWindowSize({ 320.f, 400.f }); // TODO: temp
			ImGui::Begin("Inspector", &m_context.isInspectorWindowOpen, ImGuiWindowFlags_NoCollapse);

			if (m_context.selectedEntity)
			{
				JNG_USER_ASSERT(m_context.selectedEntity.hasComponent<TagComponent>(), "TagComponent is obligatory!");
				auto& tag = m_context.selectedEntity.getComponent<TagComponent>().tag;

				char buffer[128];
				strcpy_s(buffer, sizeof(buffer), tag.c_str());
				if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
					tag = buffer;
				ImGui::Separator();

				updateComponent<TransformComponent>("Transform", m_context.selectedEntity,
					[](TransformComponent& tc) {
						ImGui::DragFloat3("Translation", glm::value_ptr(tc.translation), 0.1f, 0.f, 0.f, "%.2f");
						ImGui::DragFloat3("Rotation", glm::value_ptr(tc.rotation), 0.1f, 0.f, 0.f, "%.2f");
						ImGui::DragFloat3("Scale", glm::value_ptr(tc.scale), 0.1f, 0.f, 0.f, "%.2f");
					}, false);

				updateComponent<CameraComponent>("Camera", m_context.selectedEntity,
					[](CameraComponent& cc) {
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
							ImGui::DragFloat("Size", &cc.orthoSize, 0.1f, 0.f, 0.f, "%.2f");
							ImGui::DragFloat("Near", &cc.orthoNear, 0.1f, 0.f, 0.f, "%.2f");
							ImGui::DragFloat("Far", &cc.orthoFar, 0.1f);
							break;
						case CameraComponent::ProjectionType::Perspective:
							ImGui::DragFloat("FOV", &cc.perspectiveFOV, 0.1f, 0.f, 0.f, "%.2f");
							ImGui::DragFloat("Near", &cc.perspectiveNear, 0.1f, 0.f, 0.f, "%.2f");
							ImGui::DragFloat("Far", &cc.perspectiveFar, 0.1f, 0.f, 0.f, "%.2f");
							break;
						}
					});

				updateComponent<SpriteComponent>("Sprite", m_context.selectedEntity,
					[](SpriteComponent& sc) {
						ImGui::ColorEdit4("Color", glm::value_ptr(sc.color));
					});

				if (ImGui::Button("Add Component"))
					ImGui::OpenPopup("AddComponent");

				if (ImGui::BeginPopup("AddComponent")) {
					if (ImGui::MenuItem("Camera")) {
						m_context.selectedEntity.addComponent<CameraComponent>();
						ImGui::CloseCurrentPopup();
					}
					else if (ImGui::MenuItem("Native Script")) {
						m_context.selectedEntity.addComponent<NativeScriptComponent>();
						ImGui::CloseCurrentPopup();
					}
					else if (ImGui::MenuItem("Sprite")) {
						m_context.selectedEntity.addComponent<SpriteComponent>();
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			ImGui::End();
		}
	}

} // namespace jng
