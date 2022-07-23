/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"
#include "jng/renderer/texture.hpp"

namespace jng {

    class Framebuffer
    {
    public:
        struct Properties {
            uint32 Width;
            uint32 Height;
            uint32 Samples = 1;
            std::vector<TextureSpecification> AttachmentsSpecifications;
            
            bool SwapChainTarget = false;
        };

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void resize(uint32 width, uint32 height) = 0;
        virtual void* getColorAttachmentHandle() = 0;
        virtual const Properties& getProperties() const = 0;

        static Ref<Framebuffer> create(const Properties& properties);
        virtual ~Framebuffer() = default;
    };

} // namespace jng
