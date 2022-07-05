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
#include <vector>

#pragma comment(lib, "d3dcompiler.lib")

namespace jng {

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

	Direct3DShader::Direct3DShader(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc) :
		m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) }
    {
		const auto& device = m_graphicsContext->getNativeDevice();

		compileShader(vertexShaderSrc.data(), Type::Vertex, m_vertexShaderByteCode);
		HRESULT hr = device->CreateVertexShader(m_vertexShaderByteCode->GetBufferPointer(), m_vertexShaderByteCode->GetBufferSize(), nullptr, &m_vertexShader);
		JNG_D3D_CHECK_HR(hr);

		wrl::ComPtr<ID3DBlob> fsByteCode;
		compileShader(fragmentShaderSrc.data(), Type::Fragment, fsByteCode);
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

	void Direct3DShader::compileShader(const char* shaderSource, Type type, wrl::ComPtr<ID3DBlob>& byteCode)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options1;
		options1.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		options1.SetOptimizationLevel(shaderc_optimization_level_performance);

		auto vulkanSpirv = compiler.CompileGlslToSpv(shaderSource, ShaderTypeToShaderC(type), "filename", options1);
		JNG_CORE_ASSERT(vulkanSpirv.GetCompilationStatus() == shaderc_compilation_status_success, vulkanSpirv.GetErrorMessage());

		// TODO: Reflect

		spirv_cross::CompilerHLSL hlslCompiler{ std::vector<uint32>{ vulkanSpirv.cbegin(), vulkanSpirv.cend() } };
		spirv_cross::CompilerHLSL::Options options2;
		options2.shader_model = 50;
		hlslCompiler.set_hlsl_options(options2);
		std::string hlslCode = hlslCompiler.compile();

		HRESULT hr;
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
	}

} // namespace jng
