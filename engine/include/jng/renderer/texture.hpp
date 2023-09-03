/*
 * Copyright (C) 2021-2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

    class Texture final
    {
    public:
        enum class Format {
            None,

            RGB8,  // normalized integer
            RGBA8, // normalized integer
            R32,   // unsigned integer

            Depth24Stencil8
        };

        enum class FilterMode {
            Linear,
            Nearest
        };

        enum class WrapMode {
            Clamp,
            Repeat
        };

        struct Properties {
            u32 width = 1;
            u32 height = 1;
            Format format = Format::None;
            FilterMode minificationFilter = FilterMode::Linear;
            FilterMode magnificationFilter = FilterMode::Linear;
            WrapMode wrapMode = WrapMode::Clamp;
        };

        explicit Texture(const char* path);
        explicit Texture(const Properties& properties);
        ~Texture();

        void bind(u32 slot) const;
        void unbind(u32 slot) const;
        void setData(void* data, size_t size) const;

        u32 getID() const { return m_id; }
        const Properties& getProperties() const { return m_properties; }
        void* getRendererID() { return reinterpret_cast<void*>(static_cast<u64>(m_id)); }
    private:
        void createTexture();

        Properties m_properties;
        u32 m_id;
    };

} // namespace jng
