/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scene_hierarchy.hpp"

#include "../editor_context.hpp"

#include <jng/scene/components.hpp>

#include <imgui.h>

namespace jng {

    void SceneHierarchyWindow::onImGuiUpdate()
    {
        if (m_context.IsSceneHierarchyWindowOpen)
        {
            ImGui::SetNextWindowSize({ 320.f, 400.f }); // TODO: this is temporary to prevent window being too small when app is started first time
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
            ImGui::Begin("Scene Hierarchy", &m_context.IsSceneHierarchyWindowOpen, ImGuiWindowFlags_NoCollapse);

            if (m_context.ActiveScene)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 7.f, 4.f });
                ImGui::BeginChild("ScneHierarchyChild", { 0, 0 }, false, ImGuiWindowFlags_AlwaysUseWindowPadding);

                m_context.ActiveScene->each([this](Entity entity)
                    {
                        if (!entity.hasParent())
                            updateSceneHierarchyItem(entity);
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

                ImGui::EndChild();
                ImGui::PopStyleVar();

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM"))
                        reinterpret_cast<Entity*>(payload->Data)->removeParent();

                    ImGui::EndDragDropTarget();
                }
            }

            ImGui::End();
            ImGui::PopStyleVar();
        }
    }

    void SceneHierarchyWindow::updateSceneHierarchyItem(Entity entity)
    {
        auto& tag = entity.getComponent<TagComponent>().Tag;
        bool hasChildren = entity.hasChildren();

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth |
            (m_context.SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) |
            (hasChildren ? 0 : ImGuiTreeNodeFlags_Leaf);

        bool isOpen = ImGui::TreeNodeEx(entity, flags, tag.c_str());

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            m_context.SelectedEntity = entity;

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("SCENE_HIERARCHY_ITEM", &entity, sizeof(Entity), ImGuiCond_Once);
            ImGui::Text(tag.c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM"))
            {
                Entity droppedEntity = *reinterpret_cast<Entity*>(payload->Data);
                droppedEntity.setParent(entity);
            }
            ImGui::EndDragDropTarget();
        }

        if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::MenuItem("Duplicate", "Ctrl+D"))
                m_context.ActiveScene->duplicateEntity(entity);

            if (ImGui::MenuItem("Delete", "Del"))
            {
                if (m_context.SelectedEntity == entity)
                    m_context.SelectedEntity = {};

                m_context.ActiveScene->destroyEntity(entity);
            }

            ImGui::EndPopup();
        }

        if (isOpen)
        {
            if (hasChildren)
            {
                auto& children = entity.getComponent<ChildrenComponent>();
                for (auto child : children.children)
                    updateSceneHierarchyItem(child);
            }

            ImGui::TreePop();
        }
    }

} // namespace jng
