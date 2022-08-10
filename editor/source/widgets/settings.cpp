/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "settings.hpp"

#include "../editor_context.hpp"

#include <jng/renderer/renderer2d.hpp>

#include <imgui.h>

namespace jng {

    void SettingsWindow::onImGuiUpdate()
    {
        if (m_context.isSettingsWindowOpen)
        {
            ImGui::SetNextWindowSize({ 320.f, 400.f }); // TODO: this is temporary to prevent window being too small when app is started first time
            ImGui::Begin("Settings", &m_context.isSettingsWindowOpen, ImGuiWindowFlags_NoCollapse);

            if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat("Gravity", &m_context.physicsGravity, 0.2f, 0.f, 0.f, "%.1f");
                ImGui::Checkbox("Show colliders", &m_context.showColliders);
            }

            ImGui::End();
        }
    }

} // namespace jng
