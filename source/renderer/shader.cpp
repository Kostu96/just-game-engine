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

#include <ccl/helper_functions.h>
#include <ccl/md5.h>
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
		std::filesystem::path shaderFilePath = shaderFilename;
		std::filesystem::path cacheDirectory = getCacheDirectory();

		size_t size;
		bool success = ccl::readFile(shaderFilename, nullptr, size, true);
		JNG_CORE_ASSERT(success, "Cannot open filename: {0}", shaderFilename);
		char* shaderSource = new char[size + 1];
		success = ccl::readFile(shaderFilename, shaderSource, size, true);
		shaderSource[size] = 0;

		// TODO: change md5 data parameter to void*
		auto checksum = ccl::md5(reinterpret_cast<uint8_t*>(shaderSource), size);

		std::filesystem::path md5Path = cacheDirectory / (shaderFilePath.stem().string() + shaderTypeToMD5FileExtension(type));

		m_isCacheDirty = false;
		success = ccl::readFile(md5Path.generic_string().c_str(), nullptr, size, true);
		if (success) {
			JNG_CORE_ASSERT(size == 16, "MD5 checksum should always be 16 bytes!");
			uint32* savedChecksum = new uint32[size / 4];
			success = ccl::readFile(md5Path.generic_string().c_str(), reinterpret_cast<char*>(savedChecksum), size, true);

			for (size_t i = 0; i < size / 4; ++i)
				if (savedChecksum[i] != checksum[i]) {
					m_isCacheDirty = true;
					break;
				}
		}
		else
			m_isCacheDirty = true;

		JNG_CORE_TRACE("Shader cache is {0}.", m_isCacheDirty ? "dirty" : "good");

		std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.stem().string() + shaderTypeToCachedVlkFileExtension(type));
		
		std::vector<uint32> vulkanSpirvData;
		if (m_isCacheDirty) {
			JNG_CORE_TRACE("Recompiling Vulkan SPIR-V...");

			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
			auto vulkanSpirv = compiler.CompileGlslToSpv(shaderSource, static_cast<shaderc_shader_kind>(shaderTypeToShaderCKind(type)), shaderFilename, options);
			JNG_CORE_ASSERT(vulkanSpirv.GetCompilationStatus() == shaderc_compilation_status_success, vulkanSpirv.GetErrorMessage());
			vulkanSpirvData = std::vector<uint32>{ vulkanSpirv.cbegin(), vulkanSpirv.cend() };

			ccl::writeFile(md5Path.generic_string().c_str(), reinterpret_cast<char*>(checksum.data()), checksum.size() * 4, true);
			ccl::writeFile(cachedPath.generic_string().c_str(), reinterpret_cast<char*>(vulkanSpirvData.data()), vulkanSpirvData.size() * sizeof(uint32), true);
		}
		else {
			JNG_CORE_TRACE("Loading Vulkan SPIR-V from cache: {0}",
				cachedPath.generic_string().c_str());

			success = ccl::readFile(cachedPath.generic_string().c_str(), nullptr, size, true);
			vulkanSpirvData.resize(size / sizeof(uint32));
			success = ccl::readFile(cachedPath.generic_string().c_str(), reinterpret_cast<char*>(vulkanSpirvData.data()), size, true);
		}

		delete[] shaderSource;

		// TODO: Reflect

		return vulkanSpirvData;
    }

	void Shader::createCacheDirectoryIfNeeded() const
	{
		auto cacheDirectory = getCacheDirectory();
		if (!std::filesystem::exists(cacheDirectory))
			std::filesystem::create_directories(cacheDirectory);
	}

	const char* Shader::shaderTypeToMD5FileExtension(Type type)
	{
		switch (type)
		{
		case Type::Vertex:    return ".md5.vert";
		case Type::Fragment:  return ".md5.frag";
		}
		JNG_CORE_ASSERT(false, "");
		return "";
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
