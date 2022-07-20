/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "main_menu_bar.hpp"

#include "../editor_layer.hpp"

#include <jng/core/engine.hpp>
#include <jng/platform/platform.hpp>
#include <jng/serializers/scene_serializer.hpp>

#include <imgui.h>

namespace jng {

    void MainMenuBar::onImGuiUpdate()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::BeginMenu("New"))
                {
                    if (ImGui::MenuItem("Project", nullptr, nullptr, false))
                    {
                        
                    }

                    if (ImGui::MenuItem("Scene"))
                    {
                        m_context.SelectedEntity = {};
                        m_context.ActiveScene = makeRef<Scene>();
                    }

                    ImGui::EndMenu();
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Open...", "Ctrl + O"))
                {
                    std::string path = Platform::openFilenameDialog("JNG Scene (*.yaml;*.yml)\0*.yaml;*.yml\0\0");
                    if (!path.empty())
                    {
                        m_context.SelectedEntity = {};
                        m_context.ActiveScene = makeRef<Scene>();
                        SceneSerializer serializer{ m_context.ActiveScene };
                        serializer.deserialize(path.c_str());
                    }
                }

                if (ImGui::MenuItem("Save...", "Ctrl + S")) 
                {
                    std::string path = Platform::saveFilenameDialog("JNG Scene (*.yaml;*.yml)\0*.yaml;*.yml\0\0");
                    if (!path.empty())
                    {
                        SceneSerializer serializer{ m_context.ActiveScene };
                        serializer.serialize(path.c_str());
                    }
                }

                ImGui::MenuItem("Save As...", "Ctrl + Shift + S", nullptr, false);

                ImGui::Separator();

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
                        m_context.ActiveScene->createEntity("Empty Entity");

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Inspector", nullptr, &m_context.IsInspectorWindowOpen);
                ImGui::MenuItem("Scene Hierarchy", nullptr, &m_context.IsSceneHierarchyWindowOpen);
                ImGui::MenuItem("Viewport", nullptr, &m_context.IsViewportWindowOpen);
                ImGui::MenuItem("Content Browser", nullptr, &m_context.IsContentBrowserWindowOpen);

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

} // namespace jng
