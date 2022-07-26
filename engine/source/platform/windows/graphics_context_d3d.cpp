/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/windows/graphics_context_d3d.hpp"

#include "core/base_internal.hpp"
#include "platform/window.hpp"
#include "platform/windows/error_checks_macros_win.hpp"

#include <d3d11.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#pragma comment(lib, "d3d11.lib")

namespace jng {

    Direct3DGraphicsContext::Direct3DGraphicsContext(Window& window) :
        m_windowHandle{ reinterpret_cast<HWND>(glfwGetWin32Window(window.getNativeWindowHandle())) }
    {
        HRESULT hr;
        
        DXGI_SWAP_CHAIN_DESC scd{};
        scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        scd.SampleDesc.Count = 1;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.BufferCount = 1;
        scd.OutputWindow = m_windowHandle;
        scd.Windowed = TRUE;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

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
        JNG_D3D_CHECK_HR(hr);

        wrl::ComPtr<ID3D11Resource> backBuffer;
        hr = m_swapChain->GetBuffer(
            0,
            __uuidof(ID3D11Resource),
            &backBuffer
        );
        JNG_D3D_CHECK_HR(hr);
        hr = m_device->CreateRenderTargetView(
            backBuffer.Get(),
            nullptr,
            &m_defaultRenderTarget
        );
        JNG_D3D_CHECK_HR(hr);

        D3D11_DEPTH_STENCIL_DESC dsd{};
        dsd.DepthEnable = true;
        dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsd.DepthFunc = D3D11_COMPARISON_LESS;
        wrl::ComPtr<ID3D11DepthStencilState> depthStencilState;
        hr = m_device->CreateDepthStencilState(&dsd, &depthStencilState);
        JNG_D3D_CHECK_HR(hr);
        m_deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

        D3D11_TEXTURE2D_DESC t2dd{};
        t2dd.Width = window.getWidth();
        t2dd.Height = window.getHeight();
        t2dd.MipLevels = 1;
        t2dd.ArraySize = 1;
        t2dd.Format = DXGI_FORMAT_D32_FLOAT;
        t2dd.SampleDesc.Count = 1;
        t2dd.SampleDesc.Quality = 0;
        t2dd.Usage = D3D11_USAGE_DEFAULT;
        t2dd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        wrl::ComPtr<ID3D11Texture2D> depthStencilTexture;
        hr = m_device->CreateTexture2D(&t2dd, nullptr, &depthStencilTexture);
        JNG_D3D_CHECK_HR(hr);

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvd{};
        dsvd.Format = t2dd.Format;
        dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvd.Texture2D.MipSlice = 0;
        hr = m_device->CreateDepthStencilView(depthStencilTexture.Get(), &dsvd, &m_defaultDepthStencil);
        JNG_D3D_CHECK_HR(hr);

        setCurrentRenderTarget(m_defaultRenderTarget.Get(), m_defaultDepthStencil.Get());

        D3D11_VIEWPORT vp{};
        vp.Width = static_cast<float>(window.getWidth());
        vp.Height = static_cast<float>(window.getHeight());
        vp.MaxDepth = 1.f;
        m_deviceContext->RSSetViewports(1, &vp);

        m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        JNG_CORE_INFO("Direct3D11 context initialized.");
    }

    // NOTE: this is needed for wrl::ComPtr to work
    Direct3DGraphicsContext::~Direct3DGraphicsContext() = default;

    void Direct3DGraphicsContext::swapBuffers() const
    {
        HRESULT hr;
        hr = m_swapChain->Present(m_isVSyncEnabled ? 1 : 0, 0);
        JNG_D3D_CHECK_HR_DEVICE_REMOVED(hr, m_device);
    }

    void Direct3DGraphicsContext::setCurrentRenderTarget(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView) const
    {
        m_currentRenderTarget = renderTargetView;
        m_currentDepthStencil = depthStencilView;
        m_deviceContext->OMSetRenderTargets(1, &m_currentRenderTarget, depthStencilView);
    }

} // namespace jng
