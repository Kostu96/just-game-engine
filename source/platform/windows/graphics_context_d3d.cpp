/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/graphics_context_d3d.hpp"

#include "platform/window.hpp"
#include "platform/windows/error_checks_macros_win.hpp"

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

namespace jng {

    Direct3DGraphicsContext::Direct3DGraphicsContext(Window& window) :
        m_window{ window },
        m_windowHandle{ reinterpret_cast<HWND>(window.getNativeWindowHandle()) }
    {
        HRESULT hr;
        
        DXGI_SWAP_CHAIN_DESC scd{};
        scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        scd.SampleDesc.Count = 1;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.BufferCount = 1;
        scd.OutputWindow = m_windowHandle;
        scd.Windowed = TRUE;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // TODO: DXGI_SWAP_EFFECT_FLIP_DISCARD https://devblogs.microsoft.com/directx/dxgi-flip-model/

        UINT deviceAndSwapChainCreationFlags = 0;

#ifdef JNG_DEBUG
        deviceAndSwapChainCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            deviceAndSwapChainCreationFlags,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &scd,
            &m_swapChain,
            &m_device,
            nullptr, 
            &m_deviceContext
        );
        K2D_D3D_CHECK_HR(hr);

        wrl::ComPtr<ID3D11Resource> backBuffer;
        hr = m_swapChain->GetBuffer(
            0,
            __uuidof(ID3D11Resource),
            &backBuffer
        );
        K2D_D3D_CHECK_HR(hr);
        hr = m_device->CreateRenderTargetView(
            backBuffer.Get(),
            nullptr,
            &m_renderTarget
        );
        K2D_D3D_CHECK_HR(hr);

        m_deviceContext->OMSetRenderTargets(1, m_renderTarget.GetAddressOf(), nullptr);

        D3D11_VIEWPORT vp{};
        vp.Width = static_cast<float>(window.getWidth());
        vp.Height = static_cast<float>(window.getHeight());
        vp.MaxDepth = 1.f;
        m_deviceContext->RSSetViewports(1, &vp);

        m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    // NOTE: this is needed for wrl::ComPtr to work
    Direct3DGraphicsContext::~Direct3DGraphicsContext() = default;

    void Direct3DGraphicsContext::makeCurrent() const
    {

    }

    void Direct3DGraphicsContext::swapBuffers() const
    {
        HRESULT hr;
        hr = m_swapChain->Present(1, 0);
        K2D_D3D_CHECK_HR_DEVICE_REMOVED(hr, m_device);
    }

} // namespace jng
