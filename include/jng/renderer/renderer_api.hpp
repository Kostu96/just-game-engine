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
    class RendererAPIImpl;

    // TODO: static?
    class RendererAPI
    {
    public:
        void setViewport(uint32 x, uint32 y, uint32 width, uint32 height) const;
        void clear(const glm::vec3& color) const;
        void drawIndexed(const VertexArray& vao) const;
        void drawIndexed(uint32 count) const;

        static Scope<RendererAPI> create();
        explicit RendererAPI(Scope<RendererAPIImpl>&& implementation);
        ~RendererAPI();
    protected:
        RendererAPI() = default;
    private:
        RendererAPI(const RendererAPI&) = delete;
        RendererAPI& operator=(const RendererAPI&) = delete;

        Scope<RendererAPIImpl> m_implementation;
    };

} // namespace jng
