/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/direct3d/shader_d3d.hpp"

#include "core/base_internal.hpp"
#include "core/engine.hpp"
#include "platform/window.hpp"
#include "platform/windows/error_checks_macros_win.hpp"

#include <glm/gtc/type_ptr.inl>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_hlsl.hpp>
#include <filesystem>
#include <ccl/helper_functions.h>
#include <vector>

#pragma comment(lib, "d3dcompiler.lib")

namespace jng {

	Direct3DShader::Direct3DShader(std::string_view vertexShaderFilename, std::string_view fragmentShaderFilename) :
		m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) }
    {
		const auto& device = m_graphicsContext->getDevice();

		createCacheDirectoryIfNeeded();

		compileShader(vertexShaderFilename.data(), Type::Vertex, m_vertexShaderByteCode);
		HRESULT hr = device->CreateVertexShader(m_vertexShaderByteCode->GetBufferPointer(), m_vertexShaderByteCode->GetBufferSize(), nullptr, &m_vertexShader);
		JNG_D3D_CHECK_HR(hr);

		wrl::ComPtr<ID3DBlob> fsByteCode;
		compileShader(fragmentShaderFilename.data(), Type::Fragment, fsByteCode);
		hr = device->CreatePixelShader(fsByteCode->GetBufferPointer(), fsByteCode->GetBufferSize(), nullptr, &m_fragmentShader);
		JNG_D3D_CHECK_HR(hr);
    }

	// NOTE: this needs to be here for com::wrl to work
	Direct3DShader::~Direct3DShader() = default;

    void Direct3DShader::bind() const
    {
		const auto& deviceContext = m_graphicsContext->getDeviceContext();

		deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		deviceContext->PSSetShader(m_fragmentShader.Get(), nullptr, 0);
    }

	std::filesystem::path Direct3DShader::getCacheDirectory() const
	{
		return Engine::get().getProperties().assetsDirectory / std::filesystem::path{ "cache/shaders/direct3d" };
	}

	void Direct3DShader::compileShader(const char* shaderFilename, Type type, wrl::ComPtr<ID3DBlob>& byteCode)
	{
		JNG_CORE_TRACE("Compiling shader: {0}", shaderFilename);

		std::vector<uint32> vulkanSpirvData = compileToVulkanSPIRV(shaderFilename, type);

		// Check for cached HLSL Intermediate
		std::filesystem::path cacheDirectory = getCacheDirectory();
		std::filesystem::path shaderFilePath = shaderFilename;
		std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.stem().string() + shaderTypeToCachedD3DFileExtension(type));
		
		HRESULT hr;
		if (m_isCacheDirty) {
			JNG_CORE_TRACE("Recompiling HLSL bytecode...");
			
			spirv_cross::CompilerHLSL hlslCompiler{ vulkanSpirvData };
			spirv_cross::CompilerHLSL::Options options2;
			options2.shader_model = 50;
			hlslCompiler.set_hlsl_options(options2);
			std::string hlslCode = hlslCompiler.compile();

			UINT compilationFlags = 0;
			compilationFlags |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
			compilationFlags |= D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef JNG_DEBUG
			compilationFlags |= D3DCOMPILE_DEBUG;
			compilationFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
			compilationFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#else
			compilationFlags |= D3DCOMPILE_SKIP_VALIDATION;
			compilationFlags |= D3DCOMPILE_PARTIAL_PRECISION;
			compilationFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

			wrl::ComPtr<ID3DBlob> errorMessages;
			hr = D3DCompile(hlslCode.c_str(), hlslCode.size(), nullptr, nullptr, nullptr, "main", shaderTypeToD3DTarget(type), compilationFlags, 0, &byteCode, &errorMessages);
			if (FAILED(hr))
			{
				if (hr == E_FAIL)
				{
					[[maybe_unused]] const char* infoLog = reinterpret_cast<const char*>(errorMessages->GetBufferPointer());
					JNG_CORE_ERROR("Vertex shader compilation failed!\n{0}", infoLog);
				}
				else
					JNG_D3D_CHECK_HR(hr);
			}

			hr = D3DWriteBlobToFile(byteCode.Get(), ccl::stringToWstring(cachedPath.generic_string()).c_str(), TRUE);
			JNG_D3D_CHECK_HR(hr);
		}
		else {
			JNG_CORE_TRACE("Loading HLSL bytecode from cache: {0}",
				cachedPath.generic_string().c_str());
			
			hr = D3DReadFileToBlob(ccl::stringToWstring(cachedPath.generic_string()).c_str(), &byteCode);
			JNG_D3D_CHECK_HR(hr);
		}
	}

	const char* Direct3DShader::shaderTypeToCachedD3DFileExtension(Type type)
	{
		switch (type)
		{
		case Type::Vertex:   return ".cached_d3d.vert";
		case Type::Fragment: return ".cached_d3d.frag";
		}
		JNG_CORE_ASSERT(false, "");
		return "";
	}

	const char* Direct3DShader::shaderTypeToD3DTarget(Type type)
	{
		switch (type)
		{
		case Type::Vertex:   return "vs_5_0";
		case Type::Fragment: return "ps_5_0";
		}
		JNG_CORE_ASSERT(false, "");
		return "";
	}

} // namespace jng
