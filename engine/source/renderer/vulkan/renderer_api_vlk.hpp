/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/renderer_api.hpp"

namespace jng {

    class VulkanRendererAPI :
        public RendererAPI::RendererAPIImpl
    {
    public:
        VulkanRendererAPI() = default;

        void setViewport(uint32 x, uint32 y, uint32 width, uint32 height) const override;
        void clear(const glm::vec3& color) const override;
        void draw(uint32 count, RendererAPI::PrimitiveType primitiveType = RendererAPI::PrimitiveType::Triangles) const override;
        void drawIndexed(uint32 count) const override;
    };

} // namespace jng
