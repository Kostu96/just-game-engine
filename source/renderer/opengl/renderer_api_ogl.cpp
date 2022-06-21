/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/opengl/renderer_api_ogl.hpp"

#include "renderer/vertex_array.hpp"
#include "renderer/buffers.hpp"

#include <glad/glad.h>

namespace jng {

    void OpenGLRendererAPI::setViewport(uint32 x, uint32 y, uint32 width, uint32 height) const
    {
        glViewport(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
    }
    
    void OpenGLRendererAPI::clear(const glm::vec3& color) const
    {
        glClearColor(color.r, color.g, color.b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vao) const
    {
        glDrawElements(GL_TRIANGLES, static_cast<int>(vao->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::drawIndexed(uint32 count) const
    {
        glDrawElements(GL_TRIANGLES, static_cast<int>(count), GL_UNSIGNED_INT, nullptr);
    }

} // namespace jng
