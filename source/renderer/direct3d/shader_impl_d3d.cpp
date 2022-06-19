/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/direct3d/shader_impl_d3d.hpp"

#include "core/base_internal.hpp"
#include "core/engine.hpp"
#include "platform/window.hpp"
#include "platform/windows/error_checks_macros_win.hpp"

#include <glm/gtc/type_ptr.inl>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace k2d {

	ShaderImpl::ShaderImpl(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc) :
		m_graphicsContext{ *Engine::get().getWindow().getGraphicsContext()->getImplementation() }
    {
		HRESULT hr;
		wrl::ComPtr<ID3DBlob> errorMessages;
		const auto& device = m_graphicsContext.getNativeDevice();

		UINT compilationFlags = 0;
		compilationFlags |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
		compilationFlags |= D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef K2D_DEBUG

		compilationFlags |= D3DCOMPILE_DEBUG;
		compilationFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
		compilationFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;

#else

		compilationFlags |= D3DCOMPILE_SKIP_VALIDATION;
		compilationFlags |= D3DCOMPILE_PARTIAL_PRECISION;
		compilationFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

#endif

		hr = D3DCompile(vertexShaderSrc.data(), vertexShaderSrc.size(), nullptr, nullptr, nullptr, "main", "vs_5_0", compilationFlags, 0, &m_vertexShaderByteCode, &errorMessages);
		if (FAILED(hr))
		{
			if (hr == E_FAIL)
			{
				[[maybe_unused]] const char* infoLog = reinterpret_cast<const char*>(errorMessages->GetBufferPointer());
				K2D_CORE_ERROR("Vertex shader compilation failed!\n{0}", infoLog);
			}
			else
				K2D_D3D_CHECK_HR(hr);
		}

		hr = device->CreateVertexShader(m_vertexShaderByteCode->GetBufferPointer(), m_vertexShaderByteCode->GetBufferSize(), nullptr, &m_vertexShader);
		K2D_D3D_CHECK_HR(hr);

		wrl::ComPtr<ID3DBlob> fsByteCode;
		hr = D3DCompile(fragmentShaderSrc.data(), fragmentShaderSrc.size(), nullptr, nullptr, nullptr, "main", "ps_5_0", compilationFlags, 0, &fsByteCode, &errorMessages);
		if (FAILED(hr))
		{
			if (hr == E_FAIL)
			{
				[[maybe_unused]] const char* infoLog = reinterpret_cast<const char*>(errorMessages->GetBufferPointer());
				K2D_CORE_ERROR("Fragment shader compilation failed!\n{0}", infoLog);
			}
			else
				K2D_D3D_CHECK_HR(hr);
		}

		hr = device->CreatePixelShader(fsByteCode->GetBufferPointer(), fsByteCode->GetBufferSize(), nullptr, &m_fragmentShader);
		K2D_D3D_CHECK_HR(hr);
    }

	ShaderImpl::~ShaderImpl() = default;

    void ShaderImpl::bind() const
    {
		const auto& deviceContext = m_graphicsContext.getNativeDeviceContext();

		deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		deviceContext->PSSetShader(m_fragmentShader.Get(), nullptr, 0);
    }

	void ShaderImpl::unbind() const
	{
		
	}

	void ShaderImpl::set(const char* /*name*/, int /*value*/) const
	{
		
	}

	void ShaderImpl::set(const char* /*name*/, const int* /*value*/, uint32 /*count*/) const
	{
		
	}

	void ShaderImpl::set(const char* /*name*/, const glm::vec3& /*value*/) const
    {
		
    }

	void ShaderImpl::set(const char* /*name*/, const glm::vec4& /*value*/) const
	{
		
	}

    void ShaderImpl::set(const char* /*name*/, const glm::mat4& /*value*/) const
    {
		
    }

} // namespace k2d
