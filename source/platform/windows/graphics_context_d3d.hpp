/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "platform/graphics_context.hpp"
#include "platform/windows/windows_base.hpp"

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

 // TODO: temp
struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

namespace jng {

    namespace wrl = Microsoft::WRL;

    class Window;

    class Direct3DGraphicsContext :
        public GraphicsContext
    {
    public:
        explicit Direct3DGraphicsContext(Window& window);
        ~Direct3DGraphicsContext();

        void makeCurrent() const override;
        void swapBuffers() const override;

        const wrl::ComPtr<ID3D11Device>& getNativeDevice() const { return m_device; }
        const wrl::ComPtr<ID3D11DeviceContext>& getNativeDeviceContext() const { return m_deviceContext; }
        const wrl::ComPtr<ID3D11RenderTargetView>& getNativeRenderTarget() const { return m_renderTarget; }
    private:
        // TODO: temp
        Window& m_window;
        HWND m_windowHandle;
        wrl::ComPtr<ID3D11Device> m_device;
        wrl::ComPtr<IDXGISwapChain> m_swapChain;
        wrl::ComPtr<ID3D11DeviceContext> m_deviceContext;
        wrl::ComPtr<ID3D11RenderTargetView> m_renderTarget;
    };

} // namespace jng
