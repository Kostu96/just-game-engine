/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

    class Texture
    {
    public:
        enum class Format {
            RGBA8,

            Depth24Stencil8
        };

        enum class Filter {
            Nearest,
            Linear
        };

        enum class WrapMode {
            Clamp,
            Wrap
        };

        struct Properties {
            Format format;
            uint32 width;
            uint32 height;
        };

        virtual void bind(uint32 slot) const = 0;
        virtual void unbind(uint32 slot) const = 0;
        virtual void setData(void* data, size_t size) const = 0;

        virtual uint32 getID() const = 0;
        virtual const Properties& getProperties() const = 0;

        static Ref<Texture> create(const char* path);
        static Ref<Texture> create(const Properties& properties);
        virtual ~Texture() = default;
    };

} // namespace jng
