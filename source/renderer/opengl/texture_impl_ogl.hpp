/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/texture.hpp"

namespace jng {

    class OpenGLTexture
    {
    public:
        OpenGLTexture(const char* path);
        OpenGLTexture(uint32 width, uint32 height);
        ~OpenGLTexture();

        void bind(uint32 slot) const;
        void unbind(uint32 slot) const;
        uint32 getWidth() const { return m_width; }
        uint32 getHeight() const { return m_height; }
        void setData(void* data, size_t size) const;

        // TODO: temp!
        uint32 getID() const { return m_id; }
    private:
        void createTexture();

        uint32 m_id;
        uint32 m_width;
        uint32 m_height;
        uint32 m_internalFormat;
        uint32 m_dataFormat;
    };

} // namespace jng
