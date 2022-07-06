/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/shader.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

#include "renderer/opengl/shader_ogl.hpp"
//#include "renderer/vulkan/shader_vlk.hpp"

#if defined(JNG_WINDOWS)
#include "renderer/direct3d/shader_d3d.hpp"
#endif

#include <cpp-common/helper_functions.h>
#include <shaderc/shaderc.hpp>
#include <filesystem>

namespace jng {
    
	Ref<Shader> Shader::create(std::string_view vertexShaderFilename, std::string_view fragmentShaderFilename)
	{
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DShader>(vertexShaderFilename, fragmentShaderFilename);
#endif
        case RendererBackend::OpenGL: return makeRef<OpenGLShader>(vertexShaderFilename, fragmentShaderFilename);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
	}

    std::vector<uint32> Shader::compileToVulkanSPIRV(const char* shaderFilename, Type type) const
    {
		shaderc::Compiler compiler;
		std::filesystem::path shaderFilePath = shaderFilename;
		std::filesystem::path cacheDirectory = getCacheDirectory();

		std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.stem().string() + shaderTypeToCachedVlkFileExtension(type));
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
			auto vulkanSpirv = compiler.CompileGlslToSpv(shaderSource, static_cast<shaderc_shader_kind>(shaderTypeToShaderCKind(type)), shaderFilename, options1);
			JNG_CORE_ASSERT(vulkanSpirv.GetCompilationStatus() == shaderc_compilation_status_success, vulkanSpirv.GetErrorMessage());
			vulkanSpirvData = std::vector<uint32>{ vulkanSpirv.cbegin(), vulkanSpirv.cend() };

			ccl::writeFile(cachedPath.generic_string().c_str(), reinterpret_cast<char*>(vulkanSpirvData.data()), vulkanSpirvData.size() * sizeof(uint32), true);
			delete[] shaderSource;
		}

		// TODO: Reflect

		return vulkanSpirvData;
    }

	void Shader::createCacheDirectoryIfNeeded() const
	{
		std::string cacheDirectory = getCacheDirectory();
		if (!std::filesystem::exists(cacheDirectory))
			std::filesystem::create_directories(cacheDirectory);
	}

	const char* Shader::shaderTypeToCachedVlkFileExtension(Type type)
	{
		switch (type)
		{
		case Type::Vertex:    return ".cached_vlk.vert";
		case Type::Fragment:  return ".cached_vlk.frag";
		}
		JNG_CORE_ASSERT(false, "");
		return "";
	}

	uint32 Shader::shaderTypeToShaderCKind(Type type)
	{
		switch (type)
		{
		case Type::Vertex:  return shaderc_glsl_vertex_shader;
		case Type::Fragment:  return shaderc_glsl_fragment_shader;
		}
		JNG_CORE_ASSERT(false, "");
		return static_cast<uint32>(-1);
	}

} // namespace jng
