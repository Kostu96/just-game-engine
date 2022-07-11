/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/jng.hpp"

namespace jng {

    class EditorLayer :
        public Layer
    {
    public:
        struct Properties
        {
            uint32 width;
            uint32 height;
        };

        explicit EditorLayer(const Properties& properties);

        void onUpdate(float dt) override;
        void onImGuiUpdate() override;
    private:
        Ref<Framebuffer> m_viewportFramebuffer;
        glm::vec2 m_viewportWindowSize{};

        Scene m_activeScene;
        OrthographicCamera m_mainCamera;
    };

} // namespace jng
