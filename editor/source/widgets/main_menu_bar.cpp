/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "main_menu_bar.hpp"

#include "../editor_context.hpp"

#include <jng/core/engine.hpp>
#include <jng/platform/platform.hpp>
#include <jng/serializers/scene_serializer.hpp>

#include <imgui.h>
#include <yaml-cpp/yaml.h>
#include <fstream>

namespace jng {

    constexpr static const char* SCENE_FILE_FILTER = "JNG Scene (*.jscn)\0*.jscn\0";

    void MainMenuBar::onImGuiUpdate()
    {
        static bool showCreateProjectPopup = false;

        if (showCreateProjectPopup)
        {
            static ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
            ImGui::SetNextWindowSize({ 600.f, 200.f });
            ImGui::SetNextWindowPos({ ImGui::GetMainViewport()->GetCenter().x - 300.f, ImGui::GetMainViewport()->GetCenter().y - 200.f });
            if (ImGui::Begin("Create Project", nullptr, flags))
            {
                static std::string directory;
                static constexpr size_t BUFFER_SIZE = 260;
                static char buffer[BUFFER_SIZE];
                if (ImGui::Button("..."))
                {
                    directory = Platform::openDirectoryDialog();
                    if (!directory.empty())
                        strcpy_s(buffer, BUFFER_SIZE, directory.c_str());
                }
                ImGui::SameLine();
                ImGui::InputText("Directory", buffer, BUFFER_SIZE);
                if (ImGui::Button("Create"))
                {
                    showCreateProjectPopup = false;
                    m_context.createProject(buffer);
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                    showCreateProjectPopup = false;
            }
            ImGui::End();
        }

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::BeginMenu("New"))
                {
                    if (ImGui::MenuItem("Project", "Ctrl+Shift+N")) showCreateProjectPopup = true;

                    if (ImGui::MenuItem("Scene", "Ctrl+N", nullptr, m_context.IsProjectOpen))
                        m_context.createScene();

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Open"))
                {
                    if (ImGui::MenuItem("Project...", "Ctrl+Shift+O"))
                    {
                        std::string directory = Platform::openDirectoryDialog();
                        if (!directory.empty())
                            m_context.openProject(directory);
                    }

                    if (ImGui::MenuItem("Scene...", "Ctrl+O", nullptr, m_context.IsProjectOpen))
                    {
                        std::string path = Platform::openFilenameDialog(SCENE_FILE_FILTER);
                        if (!path.empty())
                            m_context.openScene(path);
                    }

                    ImGui::EndMenu();
                }
                
                ImGui::Separator();

                if (ImGui::MenuItem("Save...", "Ctrl+S"))
                {
                    if (!m_context.EditorScenePath.empty())
                        m_context.saveScene(m_context.EditorScenePath);
                    else
                    {
                        std::string path = Platform::saveFilenameDialog(SCENE_FILE_FILTER);
                        m_context.saveScene(path);
                    }
                }

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                {
                    std::string path = Platform::saveFilenameDialog(SCENE_FILE_FILTER);
                    m_context.saveScene(path);
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Exit")) Engine::get().close();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::MenuItem("Undo", "Ctrl+Z", nullptr, false);
                ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, false);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Scene", static_cast<bool>(m_context.ActiveScene)))
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
                if (ImGui::BeginMenu("Windows"))
                {
                    ImGui::MenuItem("Inspector", nullptr, &m_context.IsInspectorWindowOpen);
                    ImGui::MenuItem("Scene Hierarchy", nullptr, &m_context.IsSceneHierarchyWindowOpen);
                    ImGui::MenuItem("Viewport", nullptr, &m_context.IsViewportWindowOpen);
                    ImGui::MenuItem("Content Browser", nullptr, &m_context.IsContentBrowserWindowOpen);
                    ImGui::MenuItem("Statistics", nullptr, &m_context.isStatisticsWindowOpen);
                    ImGui::MenuItem("Settings", nullptr, &m_context.isSettingsWindowOpen);
                
                    ImGui::EndMenu();
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Reset camera"))
                    m_context.EditorCamera.reset();

                ImGui::Separator();

                ImGui::MenuItem("No Gizmo", "Q", nullptr, false);
                ImGui::MenuItem("Translate", "W", nullptr, false);
                ImGui::MenuItem("Rotate", "E", nullptr, false);
                ImGui::MenuItem("Scale", "R", nullptr, false);

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

} // namespace jng
