/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vulkan/shader_vlk.hpp"

#include "core/base_internal.hpp"
#include "core/engine.hpp"

namespace jng {

	VulkanShader::VulkanShader(std::string_view /*vertexShaderFilename*/, std::string_view /*fragmentShaderFilename*/)
    {
		m_id = 0;
    }

	VulkanShader::~VulkanShader()
    {
		
    }

    void VulkanShader::bind() const
    {
		
    }

	void VulkanShader::unbind() const
	{
		
	}

	std::filesystem::path VulkanShader::getCacheDirectory() const
	{
		return Engine::get().getProperties().assetsDirectory / std::filesystem::path{ "cache/shaders/vulkan" };
	}

	uint32 VulkanShader::compileShader(const char* /*shaderSource*/, uint32 /*shaderType*/) const
	{
		return 0;
	}

} // namespace jng
