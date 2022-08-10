/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/renderer_api.hpp"

#include "renderer/buffers.hpp"
#include "renderer/vertex_array.hpp"

#include <glad/gl.h>

namespace jng::RendererAPI {

    static GLenum primitiveTypeToGLenum(RendererAPI::PrimitiveType primitiveType)
    {
        switch (primitiveType)
        {
        case RendererAPI::PrimitiveType::Lines:     return GL_LINES;
        case RendererAPI::PrimitiveType::Triangles: return GL_TRIANGLES;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return static_cast<GLenum>(-1);
    }

    void setViewport(uint32 x, uint32 y, uint32 width, uint32 height)
    {
        glViewport(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
    }
    
    void clear(const glm::vec3& color)
    {
        glClearColor(color.r, color.g, color.b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void draw(uint32 count, RendererAPI::PrimitiveType primitiveType)
    {
        glDrawArrays(primitiveTypeToGLenum(primitiveType), 0, static_cast<GLsizei>(count));
    }

    void drawIndexed(uint32 count)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
    }

} // namespace jng::RendererAPI
