/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/direct3d/texture_d3d.hpp"

#include "core/base_internal.hpp"
#include "core/engine.hpp"
#include "platform/window.hpp"
#include "platform/windows/error_checks_macros_win.hpp"

#include <d3d11.h>
#include <stb/stb_image.h>

namespace jng {

	Direct3DTexture::Direct3DTexture(const char* path) :
		m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) }
    {
		int width, height, channels;
		//stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path, &width, &height, &channels, 4);
		JNG_CORE_ASSERT(data, "Failed to load image: {0}", path);

		m_width = static_cast<uint32>(width);
		m_height = static_cast<uint32>(height);

		createTexture(data);

		stbi_image_free(data);
    }

	Direct3DTexture::Direct3DTexture(uint32 width, uint32 height) :
		m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) },
		m_width(width),
		m_height(height)
	{
		createTexture(nullptr);
	}

	// NOTE: this needs to be here for com::wrl to work
	Direct3DTexture::~Direct3DTexture() = default;

    void Direct3DTexture::bind(uint32 slot) const
    {
		const auto& deviceContext = m_graphicsContext->getNativeDeviceContext();
		deviceContext->PSSetShaderResources(slot, 1, m_textureView.GetAddressOf());
		deviceContext->PSSetSamplers(slot, 1, m_sampler.GetAddressOf());
    }

	void Direct3DTexture::unbind(uint32 /*slot*/) const
	{
		
	}

	void Direct3DTexture::setData(void* /*data*/, [[maybe_unused]] size_t size) const
	{
		
	}

	void Direct3DTexture::createTexture(void* data)
	{
		const auto& device = m_graphicsContext->getNativeDevice();

		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = m_width;
		textureDesc.Height = m_height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = data;
		sd.SysMemPitch = m_width * 4;

		wrl::ComPtr<ID3D11Texture2D> pTexture;
		HRESULT hr = device->CreateTexture2D(&textureDesc, data ? &sd : nullptr, &pTexture);
		JNG_D3D_CHECK_HR(hr);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = static_cast<UINT>(-1);
		hr = device->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_textureView);
		JNG_D3D_CHECK_HR(hr);

		D3D11_SAMPLER_DESC samplerDesc{};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		hr = device->CreateSamplerState(&samplerDesc, &m_sampler);
		JNG_D3D_CHECK_HR(hr);
	}

} // namespace jng
