/*
 * Copyright (C) 2021-2023 Konstanty Misiak
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

    enum class IndexType {
        U8,
        U16,
        U32
    };
        
    void setViewport(u32 x, u32 y, u32 width, u32 height);
    void clear(const glm::vec3& color);
    void draw(u32 count, PrimitiveType primitiveType = PrimitiveType::Triangles);
    void drawIndexed(u32 count, IndexType indexType, PrimitiveType primitiveType = PrimitiveType::Triangles);

} // namespace jng::RendererAPI
