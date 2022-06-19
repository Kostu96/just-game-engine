/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vulkan/renderer_api_impl_vlk.hpp"

#include "renderer/vertex_array.hpp"
#include "renderer/buffers.hpp"

#include <glad/glad.h>

namespace k2d {

    void RendererAPIImpl::setViewport(uint32 x, uint32 y, uint32 width, uint32 height) const
    {
        glViewport(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
    }
    
    void RendererAPIImpl::clear(const glm::vec3& color) const
    {
        glClearColor(color.r, color.g, color.b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RendererAPIImpl::drawIndexed(const VertexArray& vao) const
    {
        glDrawElements(GL_TRIANGLES, static_cast<int>(vao.getIndexBuffer()->getCount()), GL_UNSIGNED_INT, nullptr);
    }

    void RendererAPIImpl::drawIndexed(uint32 count) const
    {
        glDrawElements(GL_TRIANGLES, static_cast<int>(count), GL_UNSIGNED_INT, nullptr);
    }

} // namespace k2d
