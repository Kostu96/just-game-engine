/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "statistics.hpp"

#include "../editor_context.hpp"

#include <jng/renderer/renderer2d.hpp>

#include <imgui.h>

namespace jng {

    void StatisticsWindow::onImGuiUpdate()
    {
        if (m_context.isStatisticsWindowOpen)
        {
            ImGui::SetNextWindowSize({ 320.f, 400.f }); // TODO: this is temporary to prevent window being too small when app is started first time
            ImGui::Begin("Statistics", &m_context.isStatisticsWindowOpen, ImGuiWindowFlags_NoCollapse);

            auto& io = ImGui::GetIO();
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            
            auto& stats = Renderer2D::getStatistics();
            ImGui::Text("Draw Calls: %d", stats.drawCalls);
            ImGui::Text("Quads: %d", stats.quadCount);
            ImGui::Text("Circles: %d", stats.circleCount);
            ImGui::Text("Lines: %d", stats.lineCount);

            ImGui::End();
        }
    }

} // namespace jng
