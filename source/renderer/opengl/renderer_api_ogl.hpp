/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/renderer_api.hpp"

namespace jng {

    class OpenGLRendererAPI :
        public RendererAPI
    {
    public:
        OpenGLRendererAPI() = default;

        void setViewportImpl(uint32 x, uint32 y, uint32 width, uint32 height) const override;
        void clearImpl(const glm::vec3& color) const override;
        void drawIndexedImpl(const VertexArray& vao) const override;
        void drawIndexedImpl(uint32 count) const override;
    };

} // namespace jng
