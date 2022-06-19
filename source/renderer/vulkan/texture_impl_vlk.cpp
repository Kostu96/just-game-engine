/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vulkan/texture_impl_vlk.hpp"

#include "core/base_internal.hpp"

#include <glad/glad.h>
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

		m_internalFormat = m_dataFormat = 0;
		if (channels == 4) {
			m_internalFormat = GL_RGBA8;
			m_dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			m_internalFormat = GL_RGB8;
			m_dataFormat = GL_RGB;
		}

		createTexture();
		glTextureSubImage2D(m_id, 0, 0, 0, static_cast<int>(m_width), static_cast<int>(m_height), m_dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
    }

	TextureImpl::TextureImpl(uint32 width, uint32 height) :
		m_width(width),
		m_height(height)
	{
		m_internalFormat = GL_RGBA8;
		m_dataFormat = GL_RGBA;

		createTexture();
	}

	TextureImpl::~TextureImpl()
    {
		glDeleteTextures(1, &m_id);
    }

    void TextureImpl::bind(uint32 slot) const
    {
		glBindTextureUnit(slot, m_id);
    }

	void TextureImpl::unbind(uint32 slot) const
	{
		glBindTextureUnit(slot, 0);
	}

	void TextureImpl::setData(void* data, [[maybe_unused]] size_t size) const
	{
		K2D_CORE_ASSERT(size == static_cast<size_t>(m_width) * static_cast<size_t>(m_height) * (m_dataFormat == GL_RGBA ? 4 : 3),
			"Data must be entire texture!");

		glTextureSubImage2D(m_id, 0, 0, 0, static_cast<int>(m_width), static_cast<int>(m_height), m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void TextureImpl::createTexture()
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		glTextureStorage2D(m_id, 1, m_internalFormat, static_cast<int>(m_width), static_cast<int>(m_height));

		glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

} // namespace k2d
