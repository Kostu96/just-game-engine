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

    class RendererAPI :
        public ccl::NonCopyable
    {
    public:
        static void init(RendererBackend backend);
        
        static void setViewport(uint32 x, uint32 y, uint32 width, uint32 height) { s_implementation->setViewportImpl(x, y, width, height); }
        static void clear(const glm::vec3& color) { s_implementation->clearImpl(color); }
        static void drawIndexed(const VertexArray& vao) { s_implementation->drawIndexedImpl(vao); }
        static void drawIndexed(uint32 count) { s_implementation->drawIndexedImpl(count); }

        virtual void setViewportImpl(uint32 x, uint32 y, uint32 width, uint32 height) const = 0;
        virtual void clearImpl(const glm::vec3& color) const = 0;
        virtual void drawIndexedImpl(const VertexArray& vao) const = 0;
        virtual void drawIndexedImpl(uint32 count) const = 0;

        virtual ~RendererAPI() = default;
    private:
        static Scope<RendererAPI> s_implementation;
    };

} // namespace jng
