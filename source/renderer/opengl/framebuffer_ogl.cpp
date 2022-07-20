/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/opengl/framebuffer_ogl.hpp"

#include "core/base_internal.hpp"

#include <glad/gl.h>

namespace jng {

    static bool isDepthAttachment(TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::RGBA8:
            return false;
        case TextureFormat::Depth24Stencil8:
            return true;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return false;
    }

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
        glViewport(0, 0, m_properties.Width, m_properties.Height);
        glClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE);
    }

    void OpenGLFramebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    }

    void OpenGLFramebuffer::resize(uint32 width, uint32 height)
    {
        m_properties.Width = width;
        m_properties.Height = height;

        recreate();
    }

    void OpenGLFramebuffer::recreate()
    {
        if (m_ID) {
            m_attachments.clear();
            glDeleteFramebuffers(1, &m_ID);
        }

        glCreateFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        m_attachments.reserve(m_properties.AttachmentsSpecifications.size());
        for (uint32 i = 0; i < m_properties.AttachmentsSpecifications.size(); ++i)
        {
            m_attachments.push_back(Texture::create({m_properties.AttachmentsSpecifications[i], m_properties.Width, m_properties.Height}));
            GLenum attachmentTarget = isDepthAttachment(m_properties.AttachmentsSpecifications[i].Format) ?
                GL_DEPTH_STENCIL_ATTACHMENT :
                GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentTarget, GL_TEXTURE_2D, m_attachments[i]->getID(), 0);
        }

        JNG_CORE_ASSERT(glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer uncomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

} // namespace jng
