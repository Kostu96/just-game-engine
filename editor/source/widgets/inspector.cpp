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

                ImGui::SameLine(contentRegionAvailable.x - 14.f);
                if (ImGui::Button("...", { 26.f, 26.f }))
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
                function(entity.getComponent<Component>());

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
                        Camera::ProjectionType selectedType = cc.camera.getProjectionType();

                        if (ImGui::BeginCombo("Projection",
                            (selectedType == Camera::ProjectionType::Orthographic) ? "Orthographic" : "Perspective"))
                        {
                            if (ImGui::Selectable("Orthographic", selectedType == Camera::ProjectionType::Orthographic))
                                cc.camera.setProjectionType(Camera::ProjectionType::Orthographic);

                            if (ImGui::Selectable("Perspective", selectedType == Camera::ProjectionType::Perspective))
                                cc.camera.setProjectionType(Camera::ProjectionType::Perspective);

                            ImGui::EndCombo();
                        }

                        switch (selectedType)
                        {
                        case Camera::ProjectionType::Orthographic:
                        {
                            float orthoSize = cc.camera.getOrthographicSize();
                            if (ImGui::DragFloat("Size", &orthoSize, 0.1f, 0.f, 0.f, "%.2f"))
                                cc.camera.setOrthographicSize(orthoSize);

                            float orthoNear = cc.camera.getOrthographicNear();
                            if (ImGui::DragFloat("Near", &orthoNear, 0.1f, 0.f, 0.f, "%.2f"))
                                cc.camera.setOrthographicNear(orthoNear);

                            float orthoFar = cc.camera.getOrthographicFar();
                            if (ImGui::DragFloat("Far", &orthoFar, 0.1f, 0.f, 0.f, "%.2f"))
                                cc.camera.setOrthographicFar(orthoFar);
                        } break;
                        case Camera::ProjectionType::Perspective:
                        {
                            float perspectiveFOV = cc.camera.getPerspectiveFOV();
                            if (ImGui::DragFloat("FOV", &perspectiveFOV, 0.1f, 0.f, 0.f, "%.2f"))
                                cc.camera.setPerspectiveFOV(perspectiveFOV);

                            float perspectiveNear = cc.camera.getPerspectiveNear();
                            if (ImGui::DragFloat("Near", &perspectiveNear, 0.1f, 0.f, 0.f, "%.2f"))
                                cc.camera.setPerspectiveNear(perspectiveNear);

                            float perspectiveFar = cc.camera.getPerspectiveFar();
                            if (ImGui::DragFloat("Far", &perspectiveFar, 0.1f, 0.f, 0.f, "%.2f"))
                                cc.camera.setPerspectiveFar(perspectiveFar);
                        } break;
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
