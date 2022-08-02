/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/opengl/framebuffer_ogl.hpp"

#include "core/base_internal.hpp"

#include <glad/gl.h>

namespace jng {

    static uint32 textureFormatToGLEnum(TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::RGBA8:           return GL_RGBA8;
        case TextureFormat::R32:             return GL_RED_INTEGER;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return 0;
    }

    static bool isDepthAttachment(TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::RGBA8:
        case TextureFormat::R32:
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

    uint32 OpenGLFramebuffer::readPixel(uint32 colorAttachmentIndex, uint32 x, uint32 y) const
    {
        JNG_CORE_ASSERT(colorAttachmentIndex < m_attachments.size(), "Index out of bounds!"); // NOTE: includes depth attachment
        glReadBuffer(GL_COLOR_ATTACHMENT0 + colorAttachmentIndex);

        uint32 pixelData;
        glReadPixels(x, y, 1, 1, textureFormatToGLEnum(m_attachments[colorAttachmentIndex]->getProperties().Specification.Format), GL_INT, &pixelData);

        return pixelData;
    }

    void OpenGLFramebuffer::clearAttachment(uint32 attachmentIndex, int value) const
    {
        const Ref<Texture>& attachment = m_attachments[attachmentIndex];
        int data[]{ value };
        glClearTexImage(
            static_cast<GLuint>(reinterpret_cast<uint64>(attachment->getRendererID())),
            0,
            textureFormatToGLEnum(attachment->getProperties().Specification.Format),
            GL_INT,
            data
        );
    }

    void OpenGLFramebuffer::clearAttachment(uint32 attachmentIndex, float value) const
    {
        const Ref<Texture>& attachment = m_attachments[attachmentIndex];
        float data[]{ value };
        glClearTexImage(
            static_cast<GLuint>(reinterpret_cast<uint64>(attachment->getRendererID())),
            1,
            textureFormatToGLEnum(attachment->getProperties().Specification.Format),
            GL_FLOAT,
            data
        );
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
        uint32 numColorAtachments = 0;
        for (uint32 i = 0; i < m_properties.AttachmentsSpecifications.size(); ++i)
        {
            bool isDepth = isDepthAttachment(m_properties.AttachmentsSpecifications[i].Format);
            if (!isDepth) numColorAtachments++;
            m_attachments.push_back(Texture::create({m_properties.AttachmentsSpecifications[i], m_properties.Width, m_properties.Height}));
            GLenum attachmentTarget =  isDepth ? GL_DEPTH_STENCIL_ATTACHMENT : GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentTarget, GL_TEXTURE_2D, m_attachments[i]->getID(), 0);
        }

        JNG_CORE_ASSERT(glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer uncomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glNamedFramebufferDrawBuffers(m_ID, numColorAtachments, buffers);
    }

} // namespace jng
