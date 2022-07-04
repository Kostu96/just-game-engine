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

#if defined(JNG_WINDOWS)
#include "platform/windows/graphics_context_d3d.hpp"

#include <backends/imgui_impl_dx11.h>
#endif

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
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D:
        {
            // TODO: temp, move thos to impl files
            const auto* graphicsContext = reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext());
            const auto& device = graphicsContext->getNativeDevice();
            const auto& deviceContext = graphicsContext->getNativeDeviceContext();
            ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
        } break;
#endif
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
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D:
            ImGui_ImplDX11_NewFrame();
            break;
#endif
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
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D:
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            break;
#endif
        case RendererBackend::OpenGL:
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            break;
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
        }

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

} // namespace jng
