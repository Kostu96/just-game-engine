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

    static GLenum primitiveTypeToGLenum(RendererAPI::PrimitiveType type)
    {
        switch (type)
        {
        case RendererAPI::PrimitiveType::Lines:     return GL_LINES;
        case RendererAPI::PrimitiveType::Triangles: return GL_TRIANGLES;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return static_cast<GLenum>(-1);
    }

    static GLenum indexTypeToGLenum(RendererAPI::IndexType type)
    {
        switch (type)
        {
        case RendererAPI::IndexType::U8:  return GL_UNSIGNED_BYTE;
        case RendererAPI::IndexType::U16: return GL_UNSIGNED_SHORT;
        case RendererAPI::IndexType::U32: return GL_UNSIGNED_INT;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return static_cast<GLenum>(-1);
    }

    void setViewport(u32 x, u32 y, u32 width, u32 height)
    {
        glViewport(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
    }
    
    void clear(const glm::vec3& color)
    {
        glClearColor(color.r, color.g, color.b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void draw(u32 count, RendererAPI::PrimitiveType primitiveType)
    {
        glDrawArrays(primitiveTypeToGLenum(primitiveType), 0, static_cast<GLsizei>(count));
    }

    void drawIndexed(u32 count, IndexType indexType, PrimitiveType primitiveType)
    {
        glDrawElements(primitiveTypeToGLenum(primitiveType), static_cast<GLsizei>(count), indexTypeToGLenum(indexType), nullptr);
    }

} // namespace jng::RendererAPI
