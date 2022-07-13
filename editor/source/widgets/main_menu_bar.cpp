/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "main_menu_bar.hpp"

#include "../editor_layer.hpp"

#include <jng/core/engine.hpp>
#include <jng/serializers/scene_serializer.hpp>

#include <imgui.h>

namespace jng {

	void MainMenuBar::onImGuiUpdate()
	{
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("New Project", nullptr, nullptr, false);

                if (ImGui::MenuItem("Open"))
                {
                    m_context.selectedEntity = {};
                    m_context.activeScene = makeRef<Scene>();
                    SceneSerializer serializer{ m_context.activeScene };
                    serializer.deserialize("test.yaml");

                    Camera* activeCamera = m_context.activeScene->getActiveCamera();
                    if (activeCamera)
                        activeCamera->setViewportSize(
                            static_cast<uint32>(m_context.viewportWindowSize.x),
                            static_cast<uint32>(m_context.viewportWindowSize.y));
                }

                if (ImGui::MenuItem("Save", "Ctrl + S")) 
                {
                    SceneSerializer serializer{ m_context.activeScene };
                    serializer.serialize("test.yaml");
                }

                ImGui::MenuItem("Save As", nullptr, nullptr, false);

                if (ImGui::MenuItem("Exit")) Engine::get().close();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::MenuItem("Undo", "Ctrl + Z", nullptr, false);
                ImGui::MenuItem("Redo", "Ctrl + Y", nullptr, false);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Scene"))
            {
                if (ImGui::BeginMenu("Create"))
                {
                    if (ImGui::MenuItem("Empty Entity"))
                        m_context.activeScene->createEntity("Empty Entity");

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Inspector", nullptr, &m_context.isInspectorWindowOpen);
                ImGui::MenuItem("Scene Hierarchy", nullptr, &m_context.isSceneHierarchyWindowOpen);
                ImGui::MenuItem("Viewport", nullptr, &m_context.isViewportWindowOpen);

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
	}

} // namespace jng
