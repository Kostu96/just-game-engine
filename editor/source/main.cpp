/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include <jng/core/engine.hpp>
#include <jng/core/entry_point.hpp>

#include "editor_layer.hpp"

#include <imgui.h>

namespace jng {

    class EditorApp :
        public jng::Engine
    {
    public:
        // These are 16:9
        static constexpr unsigned int WindowWidth = 1744;
        static constexpr unsigned int WindowHeight = 981;

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
