/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

#include <glm/glm.hpp>

namespace jng {

    class VertexArray;

    // TODO: static?
    class RendererAPI
    {
    public:
        virtual void setViewport(uint32 x, uint32 y, uint32 width, uint32 height) const = 0;
        virtual void clear(const glm::vec3& color) const = 0;
        virtual void drawIndexed(const VertexArray& vao) const = 0;
        virtual void drawIndexed(uint32 count) const = 0;

        static Scope<RendererAPI> create();
        virtual ~RendererAPI() = default;
    protected:
        RendererAPI() = default;
    private:
        RendererAPI(const RendererAPI&) = delete;
        RendererAPI& operator=(const RendererAPI&) = delete;
    };

} // namespace jng
