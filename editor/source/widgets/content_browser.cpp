/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "content_browser.hpp"

#include "../editor_layer.hpp"

#include <imgui.h>

namespace jng {
    
    void ContentBrowserWindow::onImGuiUpdate()
    {
        if (m_context.IsContentBrowserWindowOpen)
        {
            static ImGuiTreeNodeFlags componentTreeNodeFlags =
                ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

            ImGui::SetNextWindowSize({ 500.f, 320.f }); // TODO: this is temporary to prevent window being too small when app is started first time
            ImGui::Begin("Content Browser", &m_context.IsContentBrowserWindowOpen, ImGuiWindowFlags_NoCollapse);

            static std::filesystem::path browsedPath = ".";

            if (browsedPath != "." && ImGui::Button("<-")) {
                browsedPath = browsedPath.parent_path();
            }

            for (auto& entry : std::filesystem::directory_iterator(browsedPath))
            {
                auto entryFilename = entry.path().filename();
                auto str = entryFilename.string();
                if (ImGui::Button(str.c_str()) && entry.is_directory())
                {
                    browsedPath /= entryFilename;
                }
            }

            ImGui::End();
        }
    }

} // namespace jng
