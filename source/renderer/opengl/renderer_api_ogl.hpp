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

        void setViewport(uint32 x, uint32 y, uint32 width, uint32 height) const override;
        void clear(const glm::vec3& color) const override;
        void drawIndexed(const VertexArray& vao) const override;
        void drawIndexed(uint32 count) const override;
    };

} // namespace jng
