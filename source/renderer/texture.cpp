/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/texture.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

//#include "renderer/direct3d/texture_impl_d3d.hpp"
#include "renderer/opengl/texture_impl_ogl.hpp"
//#include "renderer/vulkan/texture_impl_vlk.hpp"

namespace jng {

    void Texture::bind(uint32 slot) const
    {
        m_implementation->bind(slot);
    }

    void Texture::unbind(uint32 slot) const
    {
        m_implementation->unbind(slot);
    }

    uint32 Texture::getWidth() const
    {
        return m_implementation->getWidth();
    }

    uint32 Texture::getHeight() const
    {
        return m_implementation->getHeight();
    }

    void Texture::setData(void* data, size_t size) const
    {
        m_implementation->setData(data, size);
    }

    uint32 Texture::getID() const
    {
        return m_implementation->getID();
    }

    Ref<Texture> Texture::create(const char* path)
	{
        JNG_PROFILE_FUNCTION();

        return makeRef<Texture>(makeScope<TextureImpl>(path));
	}

    Ref<Texture> Texture::create(uint32 width, uint32 height)
    {
        JNG_PROFILE_FUNCTION();

        return makeRef<Texture>(makeScope<TextureImpl>(width, height));
    }

    Texture::Texture(Scope<TextureImpl>&& implementation) :
        m_implementation{ std::move(implementation) } {}

    Texture::~Texture() = default;

} // namespace jng
