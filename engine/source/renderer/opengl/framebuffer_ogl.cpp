/*
 * Copyright (C) 2022-2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/framebuffer.hpp"

#include "core/base_internal.hpp"

#include <glad/gl.h>

namespace jng {

    static u32 textureFormatToGLEnum(Texture::Format format)
    {
        switch (format)
        {
        case Texture::Format::RGBA8: return GL_RGBA8;
        case Texture::Format::R32:   return GL_RED_INTEGER;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return 0;
    }

    static bool isDepthAttachment(Texture::Format format)
    {
        switch (format)
        {
        case Texture::Format::RGBA8:
        case Texture::Format::R32:
            return false;
        case Texture::Format::Depth24Stencil8:
            return true;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return false;
    }

    Framebuffer::Framebuffer(const Properties& properties) :
        m_properties{ properties }
    {
        recreate();
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteTextures(1, &m_colorAttachmentID);
        glDeleteTextures(1, &m_depthAttachmentID);
        glDeleteFramebuffers(1, &m_ID);
    }

    void Framebuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        glViewport(0, 0, m_properties.width, m_properties.height);
        glClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE);
    }

    void Framebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    }

    void Framebuffer::resize(u32 width, u32 height)
    {
        m_properties.width = width;
        m_properties.height = height;

        recreate();
    }

    u32 Framebuffer::readPixel(u32 colorAttachmentIndex, u32 x, u32 y) const
    {
        JNG_CORE_ASSERT(colorAttachmentIndex < m_attachments.size(), "Index out of bounds!"); // NOTE: includes depth attachment
        glReadBuffer(GL_COLOR_ATTACHMENT0 + colorAttachmentIndex);

        u32 pixelData;
        glReadPixels(x, y, 1, 1, textureFormatToGLEnum(m_attachments[colorAttachmentIndex]->getProperties().format), GL_INT, &pixelData);

        return pixelData;
    }

    void Framebuffer::clearAttachment(u32 attachmentIndex, int value) const
    {
        const Ref<Texture>& attachment = m_attachments[attachmentIndex];
        int data[]{ value };
        glClearTexImage(
            static_cast<GLuint>(reinterpret_cast<u64>(attachment->getRendererID())),
            0,
            textureFormatToGLEnum(attachment->getProperties().format),
            GL_INT,
            data
        );
    }

    void Framebuffer::clearAttachment(u32 attachmentIndex, float value) const
    {
        const Ref<Texture>& attachment = m_attachments[attachmentIndex];
        float data[]{ value };
        glClearTexImage(
            static_cast<GLuint>(reinterpret_cast<u64>(attachment->getRendererID())),
            1,
            textureFormatToGLEnum(attachment->getProperties().format),
            GL_FLOAT,
            data
        );
    }

    void Framebuffer::recreate()
    {
        if (m_ID) {
            m_attachments.clear();
            glDeleteFramebuffers(1, &m_ID);
        }

        glCreateFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        m_attachments.reserve(m_properties.attachmentsProperties.size());
        u32 numColorAtachments = 0;
        for (u32 i = 0; i < m_properties.attachmentsProperties.size(); ++i)
        {
            bool isDepth = isDepthAttachment(m_properties.attachmentsProperties[i].format);
            if (!isDepth) numColorAtachments++;
            m_properties.attachmentsProperties[i].width = m_properties.width;
            m_properties.attachmentsProperties[i].height = m_properties.height;
            m_attachments.push_back(makeRef<Texture>(m_properties.attachmentsProperties[i]));
            GLenum attachmentTarget =  isDepth ? GL_DEPTH_STENCIL_ATTACHMENT : GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentTarget, GL_TEXTURE_2D, m_attachments[i]->getID(), 0);
        }

        JNG_CORE_ASSERT(glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer uncomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        JNG_CORE_ASSERT(numColorAtachments <= 4, "More that 4 color attachments are unsuppoerted!");
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glNamedFramebufferDrawBuffers(m_ID, numColorAtachments, buffers);
    }

} // namespace jng
