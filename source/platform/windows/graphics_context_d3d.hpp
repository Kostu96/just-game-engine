/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "platform/graphics_context.hpp"
#include "platform/windows/windows_base.hpp"

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

        void setVSync(bool enabled) override { m_isVSyncEnabled = enabled; }

        void swapBuffers() const override;

        const wrl::ComPtr<ID3D11Device>& getDevice() const { return m_device; }
        const wrl::ComPtr<ID3D11DeviceContext>& getDeviceContext() const { return m_deviceContext; }
        ID3D11RenderTargetView* getCurrentRenderTarget() const { return m_currentRenderTarget; }
        ID3D11RenderTargetView* getDefaultRenderTarget() const { return m_defaultRenderTarget.Get(); }

        void setCurrentRenderTarget(ID3D11RenderTargetView* renderTargetView) const;
    private:
        HWND m_windowHandle;
        wrl::ComPtr<ID3D11Device> m_device;
        wrl::ComPtr<IDXGISwapChain> m_swapChain;
        wrl::ComPtr<ID3D11DeviceContext> m_deviceContext;
        wrl::ComPtr<ID3D11RenderTargetView> m_defaultRenderTarget;
        mutable ID3D11RenderTargetView* m_currentRenderTarget = nullptr;
        bool m_isVSyncEnabled = false;
    };

} // namespace jng
