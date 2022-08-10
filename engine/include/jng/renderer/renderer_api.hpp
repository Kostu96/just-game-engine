/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

#include <glm/glm.hpp>

namespace jng::RendererAPI {

    enum class PrimitiveType {
        Lines,
        Triangles
    };
        
    void setViewport(uint32 x, uint32 y, uint32 width, uint32 height);
    void clear(const glm::vec3& color);
    void draw(uint32 count, PrimitiveType primitiveType = PrimitiveType::Triangles);
    void drawIndexed(uint32 count);

} // namespace jng::RendererAPI
