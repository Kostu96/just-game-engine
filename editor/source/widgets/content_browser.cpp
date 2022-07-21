/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "content_browser.hpp"

#include "../editor_layer.hpp"

#include <imgui.h>

namespace jng {

    ContentBrowserWindow::ContentBrowserWindow(EditorContext& context) :
        m_context{ context },
        m_fileIcon{ Texture::create("assets/textures/file_icon.png") },
        m_directoryIcon{ Texture::create("assets/textures/directory_icon.png") }
    {}

    void ContentBrowserWindow::onImGuiUpdate()
    {
        if (m_context.IsContentBrowserWindowOpen)
        {
            static ImGuiTreeNodeFlags componentTreeNodeFlags =
                ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

            ImGui::SetNextWindowSize({ 500.f, 320.f }); // TODO: this is temporary to prevent window being too small when app is started first time
            ImGui::Begin("Content Browser", &m_context.IsContentBrowserWindowOpen, ImGuiWindowFlags_NoCollapse);

            static std::filesystem::path browsedPath = m_context.ProjectPath;

            if (browsedPath != m_context.ProjectPath && ImGui::Button("<--")) {
                browsedPath = browsedPath.parent_path();
            }

            static float padding = 16.f;
            static float thumbnailSize = 84.f;
            float windowWidth = ImGui::GetContentRegionAvail().x;
            int columns = static_cast<int>(windowWidth / (thumbnailSize + padding));
            if (columns < 1) columns = 1;
            else if (columns > 64) columns = 64;

            if (ImGui::BeginTable("content", columns))
            {
                for (auto& entry : std::filesystem::directory_iterator(browsedPath))
                {
                    ImGui::TableNextColumn();

                    auto entryFilename = entry.path().filename();
                    auto str = entryFilename.string();

                    Ref<Texture> thumbnail = entry.is_directory() ? m_directoryIcon : m_fileIcon;
                    ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
                    ImGui::ImageButton(thumbnail->getRendererID(), { thumbnailSize, thumbnailSize });
                    
                    ImGui::PopStyleColor();
                    if (ImGui::IsItemHovered() &&
                        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) &&
                        entry.is_directory())
                    {
                        browsedPath /= entryFilename;
                    }
                    ImGui::TextWrapped(str.c_str());
                }

                ImGui::EndTable();
            }
            ImGui::End();
        }
    }

} // namespace jng
