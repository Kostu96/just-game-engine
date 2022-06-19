/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/direct3d/renderer_api_impl_d3d.hpp"

#include "core/engine.hpp"
#include "platform/graphics_context_d3d.hpp"
#include "platform/window.hpp"
#include "platform/windows/windows_base.hpp"
#include "platform/windows/error_checks_macros_win.hpp"

#include <glm/gtc/type_ptr.inl>
#include <d3d11.h>

namespace k2d {

    void RendererAPIImpl::setViewport(uint32 /*x*/, uint32 /*y*/, uint32 /*width*/, uint32 /*height*/) const
    {
        // TODO: implement
    }

    void RendererAPIImpl::clear(const glm::vec3& color) const
    {
        const auto* graphicsContext = Engine::get().getWindow().getGraphicsContext()->getImplementation();
        const auto& deviceContext = graphicsContext->getNativeDeviceContext();
        const auto& renderTarget = graphicsContext->getNativeRenderTarget();

        deviceContext->ClearRenderTargetView(renderTarget.Get(), glm::value_ptr(color));
        K2D_D3D_CHECK();
    }

    void RendererAPIImpl::drawIndexed(const VertexArray& /*vao*/) const
    {
        // TODO: implement
    }

    void RendererAPIImpl::drawIndexed(uint32 count) const
    {
        const auto* graphicsContext = Engine::get().getWindow().getGraphicsContext()->getImplementation();
        const auto& deviceContext = graphicsContext->getNativeDeviceContext();

        deviceContext->DrawIndexed(count, 0, 0);
        K2D_D3D_CHECK();
    }

} // namespace k2d
