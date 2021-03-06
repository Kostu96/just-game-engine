/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/opengl/framebuffer_ogl.hpp"

#include "core/base_internal.hpp"

#include <glad/gl.h>

namespace jng {

	OpenGLFramebuffer::OpenGLFramebuffer(const Properties& properties) :
		m_properties{ properties }
	{
		recreate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteTextures(1, &m_colorAttachmentID);
		glDeleteTextures(1, &m_depthAttachmentID);
		glDeleteFramebuffers(1, &m_ID);
	}

	void OpenGLFramebuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glViewport(0, 0, m_properties.width, m_properties.height);
		glClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE);
	}

	void OpenGLFramebuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	}

	void OpenGLFramebuffer::resize(uint32 width, uint32 height)
	{
		m_properties.width = width;
		m_properties.height = height;

		recreate();
	}

	void OpenGLFramebuffer::recreate()
	{
		if (m_ID) {
			glDeleteTextures(1, &m_colorAttachmentID);
			glDeleteTextures(1, &m_depthAttachmentID);
			glDeleteFramebuffers(1, &m_ID);
		}

		glCreateFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachmentID);
		glBindTexture(GL_TEXTURE_2D, m_colorAttachmentID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_properties.width, m_properties.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTextureParameteri(m_colorAttachmentID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_colorAttachmentID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachmentID, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachmentID);
		glBindTexture(GL_TEXTURE_2D, m_depthAttachmentID);
		glTextureStorage2D(m_depthAttachmentID, 1, GL_DEPTH24_STENCIL8, m_properties.width, m_properties.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachmentID, 0);

		JNG_CORE_ASSERT(glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer uncomplete!");

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

} // namespace jng
