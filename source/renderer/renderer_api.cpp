/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/renderer_api.hpp"

#include "core/base_internal.hpp"


#include "renderer/direct3d/renderer_api_impl_d3d.hpp"
// TODO: this
#if defined(JNG_RENDER_API_OPENGL)
    #include "renderer/opengl/renderer_api_impl_ogl.hpp"
#elif defined(JNG_RENDER_API_VULKAN)
    #include "renderer/vulkan/renderer_api_impl_vlk.hpp"
#endif

namespace jng {

    void RendererAPI::setViewport(uint32 x, uint32 y, uint32 width, uint32 height) const
    {
        m_implementation->setViewport(x, y, width, height);
    }

    void RendererAPI::clear(const glm::vec3& color) const
    {
        m_implementation->clear(color);
    }

    void RendererAPI::drawIndexed(const VertexArray& vao) const
    {
        m_implementation->drawIndexed(vao);
    }

    void RendererAPI::drawIndexed(uint32 count) const
    {
        m_implementation->drawIndexed(count);
    }

    Scope<RendererAPI> RendererAPI::create()
    {
        return makeScope<RendererAPI>(makeScope<RendererAPIImpl>());
    }

    RendererAPI::RendererAPI(Scope<RendererAPIImpl>&& implementation) :
        m_implementation{ std::move(implementation) } {}

    RendererAPI::~RendererAPI() = default;

} // namespace jng
