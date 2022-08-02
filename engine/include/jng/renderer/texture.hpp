/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

    enum class TextureFormat {
        None,

        RGBA8, // normalized integer
        R32,   // unsigned integer

        Depth24Stencil8
    };

    enum class TextureFilter {
        Linear,
        Nearest
    };

    enum class TextureWrapMode {
        Clamp,
        Wrap
    };

    struct TextureSpecification {
        TextureFormat Format;
        TextureFilter MinificationFilter = TextureFilter::Linear;
        TextureFilter MagnificationFilter = TextureFilter::Linear;
        TextureWrapMode WrapMode = TextureWrapMode::Clamp;

        TextureSpecification(TextureFormat format) : Format{ format } {}
    };

    class Texture
    {
    public:
        struct Properties {
            TextureSpecification Specification = TextureFormat::None;
            uint32 Width;
            uint32 Height;
        };

        virtual void bind(uint32 slot) const = 0;
        virtual void unbind(uint32 slot) const = 0;
        virtual void setData(void* data, size_t size) const = 0;

        virtual uint32 getID() const = 0;
        virtual const Properties& getProperties() const = 0;
        virtual void* getRendererID() = 0;

        static Ref<Texture> create(const char* path);
        static Ref<Texture> create(const Properties& properties);
        virtual ~Texture() = default;
    };

} // namespace jng
