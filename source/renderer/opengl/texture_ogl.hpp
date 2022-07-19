/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/texture.hpp"

namespace jng {

    class OpenGLTexture :
        public Texture
    {
    public:
        explicit OpenGLTexture(const char* path);
        explicit OpenGLTexture(const Properties& properties);
        virtual ~OpenGLTexture();

        void bind(uint32 slot) const override;
        void unbind(uint32 slot) const override;
        void setData(void* data, size_t size) const override;

        uint32 getID() const override { return m_id; }
        const Properties& getProperties() const override { return m_properties; }
    private:
        void createTexture();

        Properties m_properties;
        uint32 m_id;
    };

} // namespace jng
