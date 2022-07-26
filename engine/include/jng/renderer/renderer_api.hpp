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

    class RendererAPI
    {
    public:
        struct RendererAPIImpl {
            virtual void setViewport(uint32 x, uint32 y, uint32 width, uint32 height) const = 0;
            virtual void clear(const glm::vec3& color) const = 0;
            virtual void draw(uint32 count) const = 0;
            virtual void drawIndexed(const Ref<VertexArray>& vao) const = 0;
            virtual void drawIndexed(uint32 count) const = 0;

            virtual ~RendererAPIImpl() = default;
        };

        static void init(RendererBackend backend);
        static RendererBackend getRendererBackend() { return s_backend; }
        
        static void setViewport(uint32 x, uint32 y, uint32 width, uint32 height) { s_implementation->setViewport(x, y, width, height); }
        static void clear(const glm::vec3& color) { s_implementation->clear(color); }
        static void draw(uint32 count) { s_implementation->draw(count); }
        static void drawIndexed(const Ref<VertexArray>& vao) { s_implementation->drawIndexed(vao); }
        static void drawIndexed(uint32 count) { s_implementation->drawIndexed(count); }
    private:
        RendererAPI() = delete;
        ~RendererAPI() = delete;

        static RendererBackend s_backend;
        static Scope<RendererAPIImpl> s_implementation;
    };

} // namespace jng
