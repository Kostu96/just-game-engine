/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/direct3d/texture_impl_d3d.hpp"

#include "core/base_internal.hpp"

#include <stb/stb_image.h>

namespace k2d {

	TextureImpl::TextureImpl(const char* path)
    {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path, &width, &height, &channels, 0);
		K2D_CORE_ASSERT(data, "Failed to load image: {0}", path)

		m_width = static_cast<uint32>(width);
		m_height = static_cast<uint32>(height);

		// impl

		stbi_image_free(data);
    }

	TextureImpl::TextureImpl(uint32 width, uint32 height) :
		m_width(width),
		m_height(height)
	{
		

		createTexture();
	}

	TextureImpl::~TextureImpl()
    {
		
    }

    void TextureImpl::bind(uint32 /*slot*/) const
    {
		
    }

	void TextureImpl::unbind(uint32 /*slot*/) const
	{
		
	}

	void TextureImpl::setData(void* /*data*/, [[maybe_unused]] size_t size) const
	{
		K2D_CORE_ASSERT(size == static_cast<size_t>(m_width) * static_cast<size_t>(m_height) * (m_dataFormat ? 4 : 3),
			"Data must be entire texture!");

	}

	void TextureImpl::createTexture()
	{
		
	}

} // namespace k2d
