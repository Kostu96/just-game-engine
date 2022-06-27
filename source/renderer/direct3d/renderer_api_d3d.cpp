/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/direct3d/renderer_api_d3d.hpp"

#include "core/engine.hpp"
#include "platform/graphics_context_d3d.hpp"
#include "platform/window.hpp"
#include "platform/windows/windows_base.hpp"
#include "platform/windows/error_checks_macros_win.hpp"
#include "renderer/buffers.hpp"
#include "renderer/vertex_array.hpp"

#include <glm/gtc/type_ptr.inl>
#include <d3d11.h>

namespace jng {

    void Direct3DRendererAPI::setViewport(uint32 /*x*/, uint32 /*y*/, uint32 /*width*/, uint32 /*height*/) const
    {
        // TODO: implement
    }

    void Direct3DRendererAPI::clear(const glm::vec3& color) const
    {
        const auto* graphicsContext = reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext());
        const auto& deviceContext = graphicsContext->getNativeDeviceContext();
        const auto& renderTarget = graphicsContext->getNativeRenderTarget();

        deviceContext->ClearRenderTargetView(renderTarget.Get(), glm::value_ptr(color));
        JNG_D3D_CHECK();
    }

    void Direct3DRendererAPI::drawIndexed(const Ref<VertexArray>& vao) const
    {
        drawIndexed(vao->getIndexBuffer()->getCount());
    }

    void Direct3DRendererAPI::drawIndexed(uint32 count) const
    {
        const auto* graphicsContext = reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext());
        const auto& deviceContext = graphicsContext->getNativeDeviceContext();

        deviceContext->DrawIndexed(count, 0, 0);
        JNG_D3D_CHECK();
    }

} // namespace jng
