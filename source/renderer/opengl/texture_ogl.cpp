/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/opengl/texture_ogl.hpp"

#include "core/base_internal.hpp"

#include <glad/gl.h>
#include <stb/stb_image.h>
#include <string>

namespace jng {

	static uint32 textureFormatToGLEnum(Texture::Format format)
	{
		switch (format)
		{
		case Texture::Format::RGBA8: return GL_RGBA8;
		case Texture::Format::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
		}

		JNG_CORE_ASSERT(false, "This should never be triggered!");
		return 0;
	}

	OpenGLTexture::OpenGLTexture(const char* path)
    {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path, &width, &height, &channels, 4);
		JNG_CORE_ASSERT(data, std::string{ "Failed to load image: " } + path);

		m_properties.format = Format::RGBA8;
		m_properties.width = static_cast<uint32>(width);
		m_properties.height = static_cast<uint32>(height);

		createTexture();
		glTextureSubImage2D(m_id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
    }

	OpenGLTexture::OpenGLTexture(const Properties& properties) :
		m_properties{ properties }
	{
		createTexture();
	}

	OpenGLTexture::~OpenGLTexture()
    {
		glDeleteTextures(1, &m_id);
    }

    void OpenGLTexture::bind(uint32 slot) const
    {
		glBindTextureUnit(slot, m_id);
    }

	void OpenGLTexture::unbind(uint32 slot) const
	{
		glBindTextureUnit(slot, 0);
	}

	void OpenGLTexture::setData(void* data, [[maybe_unused]] size_t size) const
	{
		JNG_CORE_ASSERT(size == static_cast<size_t>(m_properties.width) * static_cast<size_t>(m_properties.height) * 4,
			"Data must be entire texture!");

		glTextureSubImage2D(m_id, 0, 0, 0, static_cast<int>(m_properties.width), static_cast<int>(m_properties.height), GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture::createTexture()
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		glTextureStorage2D(m_id, 1, textureFormatToGLEnum(m_properties.format), static_cast<int>(m_properties.width), static_cast<int>(m_properties.height));

		glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

} // namespace jng
