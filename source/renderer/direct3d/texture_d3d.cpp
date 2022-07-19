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
#include "platform/windows/graphics_context_d3d.hpp"

#include <d3d11.h>
#include <stb/stb_image.h>

namespace jng {

	static DXGI_FORMAT textureFormatToDXGIFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGBA8: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case TextureFormat::Depth24Stencil8: return DXGI_FORMAT_R24G8_TYPELESS;
		}

		JNG_CORE_ASSERT(false, "This should never be triggered!");
		return DXGI_FORMAT_UNKNOWN;
	}

	Direct3DTexture::Direct3DTexture(const char* path) :
		m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) },
		m_ID{ s_ID++ }
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path, &width, &height, &channels, 4);
		JNG_CORE_ASSERT(data, std::string{ "Failed to load image: " } + path);

		m_properties.Specification.Format = TextureFormat::RGBA8;
		m_properties.Width = static_cast<uint32>(width);
		m_properties.Height = static_cast<uint32>(height);

		createTexture(data);

		stbi_image_free(data);
	}

	Direct3DTexture::Direct3DTexture(const Properties& properties) :
		m_properties{ properties },
		m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) },
		m_ID{ s_ID++ }
	{
		createTexture(nullptr);
	}

	// NOTE: this needs to be here for wrl::ComPtr to work
	Direct3DTexture::~Direct3DTexture() = default;

    void Direct3DTexture::bind(uint32 slot) const
    {
		const auto& deviceContext = m_graphicsContext->getDeviceContext();
		deviceContext->PSSetShaderResources(slot, 1, m_textureView.GetAddressOf());
		deviceContext->PSSetSamplers(slot, 1, m_sampler.GetAddressOf());
    }

	void Direct3DTexture::unbind(uint32 /*slot*/) const
	{
		
	}

	void Direct3DTexture::setData(void* data, size_t size) const
	{
		const auto& deviceContext = m_graphicsContext->getDeviceContext();

		wrl::ComPtr<ID3D11Resource> resource;
		m_textureView->GetResource(&resource);
		D3D11_MAPPED_SUBRESOURCE mappedResource{};
		[[ maybe_unused ]] HRESULT hr = deviceContext->Map(resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		JNG_D3D_CHECK_HR(hr);
		memcpy(mappedResource.pData, data, size);
		deviceContext->Unmap(resource.Get(), 0);
	}

	void Direct3DTexture::createTexture(void* data)
	{
		const auto& device = m_graphicsContext->getDevice();

		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = m_properties.Width;
		textureDesc.Height = m_properties.Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = textureFormatToDXGIFormat(m_properties.Specification.Format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = data ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = data ? 0 : D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = data;
		sd.SysMemPitch = m_properties.Width * 4;

		wrl::ComPtr<ID3D11Texture2D> pTexture;
		[[ maybe_unused ]] HRESULT hr = device->CreateTexture2D(&textureDesc, data ? &sd : nullptr, &pTexture);
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

	uint32 Direct3DTexture::s_ID = 0;

} // namespace jng
