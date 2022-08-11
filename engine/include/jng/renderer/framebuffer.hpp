/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"
#include "jng/renderer/texture.hpp"

namespace jng {

    class Framebuffer final
    {
    public:
        struct Properties {
            uint32 width;
            uint32 height;
            uint32 samples = 1;
            std::vector<TextureSpecification> attachmentsSpecifications;
            
            bool swapChainTarget = false;
        };

        explicit Framebuffer(const Properties& properties);
        ~Framebuffer();

        void bind() const;
        void unbind() const;

        void resize(uint32 width, uint32 height);
        uint32 readPixel(uint32 colorAttachmentIndex, uint32 x, uint32 y) const;
        void clearAttachment(uint32 attachmentIndex, int value) const;
        void clearAttachment(uint32 attachmentIndex, float value) const;
        
        const std::vector<Ref<Texture>>& getAttachments() const { return m_attachments; }
        const Properties& getProperties() const { return m_properties; }
    private:
        void recreate();

        Properties m_properties;
        uint32 m_ID;
        std::vector<Ref<Texture>> m_attachments;
        uint32 m_colorAttachmentID;
        uint32 m_depthAttachmentID;
    };

} // namespace jng
