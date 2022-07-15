/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vulkan/texture_vlk.hpp"

#include "core/base_internal.hpp"

#include <stb/stb_image.h>

namespace jng {

	VulkanTexture::VulkanTexture(const char* /*path*/)
    {
		m_id = 0;
		m_width = 0;
		m_height = 0;
		m_internalFormat = 0;
		m_dataFormat = 0;
    }

	VulkanTexture::VulkanTexture(uint32 width, uint32 height) :
		m_width(width),
		m_height(height)
	{
		m_internalFormat = 0;
		m_dataFormat = 0;

		createTexture();
	}

	VulkanTexture::~VulkanTexture()
    {
		
    }

    void VulkanTexture::bind(uint32 /*slot*/) const
    {
		
    }

	void VulkanTexture::unbind(uint32 /*slot*/) const
	{
		
	}

	void VulkanTexture::setData(void* /*data*/, size_t /*size*/) const
	{
		
	}

	void VulkanTexture::createTexture()
	{
		
	}

} // namespace k2d
