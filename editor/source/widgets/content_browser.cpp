/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "content_browser.hpp"

#include "../editor_context.hpp"

#include <imgui.h>

namespace jng {

    static const char* fileExtensionToPayloadID(const std::filesystem::path& extension)
    {
        // NOTE: these can be max 32 characters long
        if (extension == L".jscn") return "CONTENT_BROWSER_ITEM_SCENE";
        if (extension == L".lua")  return "CONTENT_BROWSER_ITEM_LUA";
        if (extension == L".png")  return "CONTENT_BROWSER_ITEM_TEXTURE";

        return "CONTENT_BROWSER_ITEM_FOLDER";
    }

    ContentBrowserWindow::ContentBrowserWindow(EditorContext& context) :
        m_context{ context },
        m_fileIcon{ makeRef<Texture>("assets/textures/file_icon.png") },
        m_directoryIcon{ makeRef<Texture>("assets/textures/directory_icon.png") }
    {}

    void ContentBrowserWindow::onImGuiUpdate()
    {
        if (m_context.IsContentBrowserWindowOpen)
        {
            static ImGuiTreeNodeFlags componentTreeNodeFlags =
                ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

            ImGui::SetNextWindowSize({ 500.f, 320.f }); // TODO: this is temporary to prevent window being too small when app is started first time
            ImGui::Begin("Content Browser", &m_context.IsContentBrowserWindowOpen, ImGuiWindowFlags_NoCollapse);

            if (m_context.BrowsedPath != m_context.AssetsPath && ImGui::Button("<--")) {
                m_context.BrowsedPath = m_context.BrowsedPath.parent_path();
            }

            static float padding = 16.f;
            static float thumbnailSize = 84.f;
            float windowWidth = ImGui::GetContentRegionAvail().x;
            int columns = static_cast<int>(windowWidth / (thumbnailSize + padding));
            if (columns < 1) columns = 1;
            else if (columns > 64) columns = 64;

            if (ImGui::BeginTable("content", columns))
            {
                int index = 0;
                for (auto& entry : std::filesystem::directory_iterator(m_context.BrowsedPath))
                {
                    ImGui::TableNextColumn();

                    auto entryFilename = entry.path().filename();
                    auto str = entryFilename.string();

                    Ref<Texture> thumbnail = entry.is_directory() ? m_directoryIcon : m_fileIcon;
                    ImGui::PushStyleColor(ImGuiCol_Button, { 0.f, 0.f, 0.f, 0.f });
                    ImGui::PushID(index++);
                    ImGui::ImageButton(thumbnail->getRendererID(), { thumbnailSize, thumbnailSize });
                    ImGui::PopID();
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
                    if (ImGui::BeginDragDropSource())
                    {
                        auto payload = entry.path().string();
                        ImGui::SetDragDropPayload(fileExtensionToPayloadID(entry.path().extension()), payload.c_str(), payload.size() + 1, ImGuiCond_Once);
                        ImGui::ImageButton(thumbnail->getRendererID(), { 32.f, 32.f });
                        ImGui::EndDragDropSource();
                    }
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();

                    if (ImGui::IsItemHovered() &&
                        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) &&
                        entry.is_directory())
                    {
                        m_context.BrowsedPath /= entryFilename;
                    }
                    ImGui::TextWrapped(str.c_str());
                }

                ImGui::EndTable();
            }
            ImGui::End();
        }
    }

} // namespace jng
