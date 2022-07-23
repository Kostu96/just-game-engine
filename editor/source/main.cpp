/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include <jng/core/engine.hpp>
#include <jng/core/entry_point.hpp>

#include "editor_layer.hpp"

#include <imgui.h>

extern "C" {
    _declspec(dllexport) uint32_t NvOptimusEnablement = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
};

namespace jng {

    class EditorApp :
        public jng::Engine
    {
    public:
        // These are 16:9
        static constexpr unsigned int WindowWidth = 1728;
        static constexpr unsigned int WindowHeight = 972;

        EditorApp() :
            Engine({
                "JNG Editor",
                WindowWidth,
                WindowHeight,
                RendererType::Renderer3D
                })
        {
            EditorLayer::Properties properties{};
            properties.width = WindowWidth;
            properties.height = WindowHeight;

            ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/lato.ttf", 18);

            getLayerStack().pushLayer(new EditorLayer{ properties });
        }
    };

}

jng::Engine* createApp() {
    return new jng::EditorApp{};
}
