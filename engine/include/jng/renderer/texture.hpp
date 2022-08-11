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
        TextureFormat format;
        TextureFilter minificationFilter = TextureFilter::Linear;
        TextureFilter magnificationFilter = TextureFilter::Linear;
        TextureWrapMode wrapMode = TextureWrapMode::Clamp;

        TextureSpecification(TextureFormat inFormat) : format{ inFormat } {}
    };

    class Texture final
    {
    public:
        struct Properties {
            TextureSpecification specification = TextureFormat::None;
            uint32 width;
            uint32 height;
        };

        explicit Texture(const char* path);
        explicit Texture(const Properties& properties);
        ~Texture();

        void bind(uint32 slot) const;
        void unbind(uint32 slot) const;
        void setData(void* data, size_t size) const;

        uint32 getID() const { return m_id; }
        const Properties& getProperties() const { return m_properties; }
        void* getRendererID() { return reinterpret_cast<void*>(static_cast<uint64>(m_id)); }
    private:
        void createTexture();

        Properties m_properties;
        uint32 m_id;
    };

} // namespace jng
