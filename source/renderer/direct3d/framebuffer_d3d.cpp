/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/direct3d/framebuffer_d3d.hpp"

#include "core/base_internal.hpp"
#include "core/engine.hpp"
#include "platform/window.hpp"
#include "platform/windows/error_checks_macros_win.hpp"
#include "platform/windows/graphics_context_d3d.hpp"

#include <d3d11.h>

namespace jng {

	Direct3DFramebuffer::Direct3DFramebuffer(const Properties& properties) :
		m_properties{ properties },
		m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) }
	{
		recreate();
	}

	// NOTE: this needs to be here for wrl::com to work
	Direct3DFramebuffer::~Direct3DFramebuffer() = default;

	void Direct3DFramebuffer::bind() const
	{
		m_graphicsContext->setCurrentRenderTarget(m_renderTargetView.Get());
	}

	void Direct3DFramebuffer::unbind() const
	{
		m_graphicsContext->setCurrentRenderTarget(m_graphicsContext->getDefaultRenderTarget());
	}

	void Direct3DFramebuffer::resize(uint32 width, uint32 height)
	{
		m_properties.Width = width;
		m_properties.Height = height;

		recreate();
	}

	void Direct3DFramebuffer::recreate()
	{
		const auto& device = m_graphicsContext->getDevice();

		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = m_properties.Width;
		textureDesc.Height = m_properties.Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;

		wrl::ComPtr<ID3D11Texture2D> pTexture;
		HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &pTexture);
		JNG_D3D_CHECK_HR(hr);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		hr = device->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_colorAttachmentView);
		JNG_D3D_CHECK_HR(hr);

		D3D11_RENDER_TARGET_VIEW_DESC renderTgtDesc{};
		renderTgtDesc.Format = textureDesc.Format;
		renderTgtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		hr = device->CreateRenderTargetView(pTexture.Get(), &renderTgtDesc, &m_renderTargetView);
		JNG_D3D_CHECK_HR(hr);
	}

} // namespace jng
