/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define JNG_DECLARE_MAIN
#include <jng/jng.hpp>

#include "editor_layer.hpp"

namespace jng {

    class EditorApp :
        public jng::Engine
    {
    public:
        static constexpr unsigned int WindowWidth = 1600;
        static constexpr unsigned int WindowHeight = 900;

        EditorApp() :
            Engine({
                "JNG Editor!",
                WindowWidth,
                WindowHeight,
                RendererType::Renderer3D
                })
        {
            EditorLayer::Properties properties{};
            properties.width = WindowWidth;
            properties.height = WindowHeight;

            getLayerStack().pushLayer(new EditorLayer{ properties });
        }
    };

}

jng::Engine* createApp() {
    return new jng::EditorApp{};
}
