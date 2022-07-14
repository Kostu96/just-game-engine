/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/texture.hpp"

#include "platform/windows/windows_base.hpp"

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

namespace jng {

    class Direct3DGraphicsContext;

    namespace wrl = Microsoft::WRL;

    class Direct3DTexture :
        public Texture
    {
    public:
        explicit Direct3DTexture(const char* path);
        Direct3DTexture(uint32 width, uint32 height);
        virtual ~Direct3DTexture();

        void bind(uint32 slot) const override;
        void unbind(uint32 slot) const override;
        uint32 getWidth() const override { return m_width; }
        uint32 getHeight() const override { return m_height; }
        void setData(void* data, size_t size) const override;

        uint32 getID() const override { return m_ID; }
    private:
        void createTexture(void* data);

        const Direct3DGraphicsContext* m_graphicsContext;
        wrl::ComPtr<ID3D11ShaderResourceView> m_textureView;
        wrl::ComPtr<ID3D11SamplerState> m_sampler;
        uint32 m_width;
        uint32 m_height;
        uint32 m_ID;
        static uint32 s_ID;
    };

} // namespace jng
