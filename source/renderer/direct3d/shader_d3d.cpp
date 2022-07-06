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
#include <cpp-common/helper_functions.h>
#include <vector>

#pragma comment(lib, "d3dcompiler.lib")

namespace jng {

	static const char* GetCacheDirectory()
	{
		return "assets/cache/shaders/direct3d";
	}

	static void CreateCacheDirectoryIfNeeded()
	{
		std::string cacheDirectory = GetCacheDirectory();
		if (!std::filesystem::exists(cacheDirectory))
			std::filesystem::create_directories(cacheDirectory);
	}

	static const char* ShaderTypeToCachedD3DFileExtension(Direct3DShader::Type type)
	{
		switch (type)
		{
		case Direct3DShader::Type::Vertex:    return ".cached_d3d.vert";
		case Direct3DShader::Type::Fragment:  return ".cached_d3d.frag";
		}
		JNG_CORE_ASSERT(false, "");
		return "";
	}

	static const char* ShaderTypeToCachedVlkFileExtension(Direct3DShader::Type type)
	{
		switch (type)
		{
		case Direct3DShader::Type::Vertex:    return ".cached_vlk.vert";
		case Direct3DShader::Type::Fragment:  return ".cached_vlk.frag";
		}
		JNG_CORE_ASSERT(false, "");
		return "";
	}

	static shaderc_shader_kind ShaderTypeToShaderC(Direct3DShader::Type type)
	{
		switch (type)
		{
		case Direct3DShader::Type::Vertex:   return shaderc_glsl_vertex_shader;
		case Direct3DShader::Type::Fragment: return shaderc_glsl_fragment_shader;
		}
		JNG_CORE_ASSERT(false, "");
		return static_cast<shaderc_shader_kind>(0);
	}

	static const char * ShaderTypeToD3DTarget(Direct3DShader::Type type)
	{
		switch (type)
		{
		case Direct3DShader::Type::Vertex:   return "vs_5_0";
		case Direct3DShader::Type::Fragment: return "ps_5_0";
		}
		JNG_CORE_ASSERT(false, "");
		return "";
	}

	Direct3DShader::Direct3DShader(std::string_view vertexShaderFilename, std::string_view fragmentShaderFilename) :
		m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) }
    {
		const auto& device = m_graphicsContext->getNativeDevice();

		CreateCacheDirectoryIfNeeded();

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
		const auto& deviceContext = m_graphicsContext->getNativeDeviceContext();

		deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		deviceContext->PSSetShader(m_fragmentShader.Get(), nullptr, 0);
    }

	// TODO: move to cpp-common
	std::wstring StringToWString(const std::string& s)
	{
		std::wstring temp(s.length(), L' ');
		std::copy(s.begin(), s.end(), temp.begin());
		return temp;
	}

	void Direct3DShader::compileShader(const char* shaderFilename, Type type, wrl::ComPtr<ID3DBlob>& byteCode)
	{
		shaderc::Compiler compiler;
		std::filesystem::path shaderFilePath = shaderFilename;
		std::filesystem::path cacheDirectory = GetCacheDirectory();

		// Check for cached Vulkan SPIR-V
		std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.stem().string() + ShaderTypeToCachedVlkFileExtension(type));
		size_t size;
		bool success = ccl::readFile(cachedPath.generic_string().c_str(), nullptr, size, true);

		std::vector<uint32> vulkanSpirvData;
		if (success) {
			vulkanSpirvData.resize(size / sizeof(uint32));
			success = ccl::readFile(cachedPath.generic_string().c_str(), reinterpret_cast<char*>(vulkanSpirvData.data()), size, true);
		}
		else {
			success = ccl::readFile(shaderFilename, nullptr, size, true);
			JNG_CORE_ASSERT(success, "Cannot open filename: {0}", shaderFilename);

			char* shaderSource = new char[size + 1];
			success = ccl::readFile(shaderFilename, shaderSource, size, true);
			shaderSource[size] = 0;

			shaderc::CompileOptions options1;
			options1.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
			options1.SetOptimizationLevel(shaderc_optimization_level_performance);

			auto vulkanSpirv = compiler.CompileGlslToSpv(shaderSource, ShaderTypeToShaderC(type), "filename", options1);
			JNG_CORE_ASSERT(vulkanSpirv.GetCompilationStatus() == shaderc_compilation_status_success, vulkanSpirv.GetErrorMessage());
			vulkanSpirvData = std::vector<uint32>{ vulkanSpirv.cbegin(), vulkanSpirv.cend() };

			ccl::writeFile(cachedPath.generic_string().c_str(), reinterpret_cast<char*>(vulkanSpirvData.data()), vulkanSpirvData.size() * sizeof(uint32), true);
			delete[] shaderSource;
		}

		// TODO: Reflect

		spirv_cross::CompilerHLSL hlslCompiler{ vulkanSpirvData };
		spirv_cross::CompilerHLSL::Options options2;
		options2.shader_model = 50;
		hlslCompiler.set_hlsl_options(options2);
		std::string hlslCode = hlslCompiler.compile();

		// Check for cached HLSL Intermediate
		cachedPath = cacheDirectory / (shaderFilePath.stem().string() + ShaderTypeToCachedD3DFileExtension(type));
		success = ccl::readFile(cachedPath.generic_string().c_str(), nullptr, size, true);

		HRESULT hr;
		if (success) {
			hr = D3DReadFileToBlob(StringToWString(cachedPath.generic_string()).c_str(), &byteCode);
			JNG_D3D_CHECK_HR(hr);
		}
		else {
			wrl::ComPtr<ID3DBlob> errorMessages;

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

			hr = D3DCompile(hlslCode.c_str(), hlslCode.size(), nullptr, nullptr, nullptr, "main", ShaderTypeToD3DTarget(type), compilationFlags, 0, &byteCode, &errorMessages);
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

			success = ccl::writeFile(cachedPath.generic_string().c_str(), reinterpret_cast<char*>(byteCode->GetBufferPointer()), byteCode->GetBufferSize(), true);
		}
	}

} // namespace jng
