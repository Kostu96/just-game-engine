/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "inspector.hpp"

#include "../editor_context.hpp"

#include <jng/core/base.hpp>
#include <jng/scene/components.hpp>
#include <jng/scripting/lua_script.hpp>

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
            std::string str_id{ label };
            str_id += "ComponenetsSettings";

            ImGui::SameLine(contentRegionAvailable.x - 14.f);
            std::string buttonID = "...##" + str_id;
            if (ImGui::Button(buttonID.c_str(), {26.f, 26.f}))
                ImGui::OpenPopup(str_id.c_str());

            auto& component = entity.getComponent<Component>();

            if (ImGui::BeginPopup(str_id.c_str()))
            {
                if (ImGui::MenuItem("Reset"))
                {
                    component.reset();
                }

                if (isRemovable && ImGui::MenuItem("Delete"))
                {
                    markForDelete = true;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if (open)
                function(component);

            if (markForDelete)
                entity.removeComponent<Component>();

            ImGui::Separator();
        }
    }

    InspectorWindow::InspectorWindow(EditorContext& context) :
        m_context{ context },
        m_checkerboard{ Texture::create("assets/textures/checkerboard.png") }
    {}

    void InspectorWindow::onImGuiUpdate()
    {
        if (m_context.IsInspectorWindowOpen)
        {
            static ImGuiTreeNodeFlags componentTreeNodeFlags =
                ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

            ImGui::SetNextWindowSize({ 320.f, 400.f }); // TODO: this is temporary to prevent window being too small when app is started first time
            ImGui::Begin("Inspector", &m_context.IsInspectorWindowOpen, ImGuiWindowFlags_NoCollapse);

            if (m_context.SelectedEntity)
            {
                JNG_USER_ASSERT(m_context.SelectedEntity.hasComponent<TagComponent>(), "TagComponent is obligatory!");
                auto& tag = m_context.SelectedEntity.getComponent<TagComponent>().Tag;

                char buffer[128];
                strcpy_s(buffer, sizeof(buffer), tag.c_str());
                if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
                    tag = buffer;
                ImGui::Separator();

                updateComponent<TransformComponent>("Transform", m_context.SelectedEntity,
                    [](TransformComponent& tc) {
                        ImGui::DragFloat3("Translation", glm::value_ptr(tc.Translation), 0.1f, 0.f, 0.f, "%.2f");
                        glm::vec3 degreesRot = glm::degrees(tc.Rotation);
                        ImGui::DragFloat3("Rotation", glm::value_ptr(degreesRot), 0.1f, 0.f, 0.f, "%.2f");
                        tc.Rotation = glm::radians(degreesRot);
                        ImGui::DragFloat3("Scale", glm::value_ptr(tc.Scale), 0.1f, 0.f, 0.f, "%.2f");
                    }, false);

                updateComponent<CameraComponent>("Camera", m_context.SelectedEntity,
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

                updateComponent<CircleRendererComponent>("Circle Renderer", m_context.SelectedEntity,
                    [this](CircleRendererComponent& crc) {
                        ImGui::ColorEdit4("Color", glm::value_ptr(crc.color));
                        ImGui::DragFloat("Thickness", &crc.thickness, 0.025f, 0.025f, 1.f);
                        ImGui::DragFloat("Fade", &crc.fade, 0.001f, 0.001f);
                    });

                updateComponent<SpriteRendererComponent>("Sprite Renderer", m_context.SelectedEntity,
                    [this](SpriteRendererComponent& src) {
                        ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
                        ImGui::Text("Texture");
                        ImGui::SameLine();
                        ImGui::ImageButton(src.texture ? src.texture->getRendererID() : m_checkerboard->getRendererID(), {64.f, 64.f});
                        if (ImGui::BeginDragDropTarget())
                        {
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                            {
                                const char* path = reinterpret_cast<const char*>(payload->Data);
                                src.texture = Texture::create(path);
                            }
                            ImGui::EndDragDropTarget();
                        }
                    });

                updateComponent<BoxCollider2DComponent>("Box Collider 2D", m_context.SelectedEntity,
                    [](BoxCollider2DComponent& bcc) {
                        ImGui::DragFloat2("Size", glm::value_ptr(bcc.Size));
                        ImGui::DragFloat("Density", &bcc.Density);
                        ImGui::DragFloat("Friction", &bcc.Friction, 0.05f, 0.f, 1.f);
                        ImGui::DragFloat("Restitution", &bcc.Restitution, 0.05f, 0.f, 1.f);
                        ImGui::DragFloat("RestitutionThreshold", &bcc.RestitutionThreshold, 0.1f, 0.f);
                    });

                updateComponent<CircleCollider2DComponent>("Circle Collider 2D", m_context.SelectedEntity,
                    [](CircleCollider2DComponent& ccc) {
                        ImGui::DragFloat("Radius", &ccc.radius);
                        ImGui::DragFloat2("Offset", glm::value_ptr(ccc.offset));
                        ImGui::DragFloat("Density", &ccc.Density);
                        ImGui::DragFloat("Friction", &ccc.Friction, 0.05f, 0.f, 1.f);
                        ImGui::DragFloat("Restitution", &ccc.Restitution, 0.05f, 0.f, 1.f);
                        ImGui::DragFloat("RestitutionThreshold", &ccc.RestitutionThreshold, 0.1f, 0.f);
                    });

                updateComponent<Rigidbody2DComponent>("Rigidbody 2D", m_context.SelectedEntity,
                    [](Rigidbody2DComponent& rbc) {
                        const char* bodyTypeStrs[] = { "Static", "Dynamic", "Kinematic" };
                        const char* currentBodyType = bodyTypeStrs[(uint32)rbc.Type];

                        if (ImGui::BeginCombo("Body Type", currentBodyType))
                        {
                            for (uint32 i = 0; i < 3; ++i)
                            {
                                bool isSelected = currentBodyType == bodyTypeStrs[i];
                                if (ImGui::Selectable(bodyTypeStrs[i], isSelected))
                                {
                                    rbc.Type = static_cast<Rigidbody2DComponent::BodyType>(i);
                                    currentBodyType = bodyTypeStrs[i];
                                }
                            }
                            ImGui::EndCombo();
                        }
                    });

                updateComponent<LuaScriptComponent>("Lua Script", m_context.SelectedEntity,
                    [](LuaScriptComponent& lsc) {
                        ImGui::Text("Script");
                        ImGui::SameLine();
                        std::string btnLabel = lsc.instance ? lsc.instance->getName() : std::string{} + "##script_button";
                        ImGui::Button(btnLabel.c_str(), { !lsc.instance ? 80.f : 0.f, 0 });

                        if (ImGui::BeginDragDropTarget())
                        {
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                            {
                                lsc.path = reinterpret_cast<const char*>(payload->Data);
                                lsc.instance = LuaScript::create(lsc.path);
                            }
                            ImGui::EndDragDropTarget();
                        }
                    });

                if (ImGui::Button("Add Component"))
                    ImGui::OpenPopup("AddComponent");

                if (ImGui::BeginPopup("AddComponent"))
                {
                    if (!m_context.SelectedEntity.hasComponent<CameraComponent>() && ImGui::MenuItem("Camera"))
                    {
                        m_context.SelectedEntity.addComponent<CameraComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    else if (!m_context.SelectedEntity.hasComponent<CircleRendererComponent>() && ImGui::MenuItem("Circle Renderer"))
                    {
                        m_context.SelectedEntity.addComponent<CircleRendererComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    else if (!m_context.SelectedEntity.hasComponent<SpriteRendererComponent>() && ImGui::MenuItem("Sprite Renderer"))
                    {
                        m_context.SelectedEntity.addComponent<SpriteRendererComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    else if (!m_context.SelectedEntity.hasComponent<BoxCollider2DComponent>() && ImGui::MenuItem("Box Collider 2D"))
                    {
                        m_context.SelectedEntity.addComponent<BoxCollider2DComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    else if (!m_context.SelectedEntity.hasComponent<CircleCollider2DComponent>() && ImGui::MenuItem("Circle Collider 2D"))
                    {
                        m_context.SelectedEntity.addComponent<CircleCollider2DComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    else if (!m_context.SelectedEntity.hasComponent<Rigidbody2DComponent>() && ImGui::MenuItem("Rigidbody 2D"))
                    {
                        m_context.SelectedEntity.addComponent<Rigidbody2DComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                    else if (!m_context.SelectedEntity.hasComponent<LuaScriptComponent>() && ImGui::MenuItem("Lua Script"))
                    {
                        m_context.SelectedEntity.addComponent<LuaScriptComponent>();
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }
            }

            ImGui::End();
        }
    }

} // namespace jng
