/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "imgui/imgui_layer.hpp"

#include "core/base_internal.hpp"
#include "core/engine.hpp"
#include "platform/window.hpp"
#include "renderer/renderer_api.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace jng {

    void ImGuiLayer::init(const Window* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOther(window->getNativeWindowHandle(), true);

        switch (RendererAPI::getRendererBackend())
        {
        case RendererBackend::OpenGL:
            ImGui_ImplOpenGL3_Init("#version 450");
            break;
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
        }
    }

    void ImGuiLayer::newFrame()
    {
        switch (RendererAPI::getRendererBackend())
        {
        case RendererBackend::OpenGL:
            ImGui_ImplOpenGL3_NewFrame();
            break;
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
        }
        
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::render()
    {
        ImGui::Render();
        
        switch (RendererAPI::getRendererBackend())
        {
        case RendererBackend::OpenGL:
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            break;
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
        }

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

} // namespace jng
