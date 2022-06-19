/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

    class TextureImpl;

    class Texture
    {
    public:
        void bind(uint32 slot) const;
        void unbind(uint32 slot) const;
        uint32 getWidth() const;
        uint32 getHeight() const;
        void setData(void* data, size_t size) const;

        // TODO: temp!
        uint32 getID() const;

        static Ref<Texture> create(const char* path);
        static Ref<Texture> create(uint32 width, uint32 height);
        explicit Texture(Scope<TextureImpl>&& implementation);
        ~Texture();
    private:
        Scope<TextureImpl> m_implementation;
    };

} // namespace jng
