/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/texture.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

//#include "renderer/direct3d/texture_impl_d3d.hpp"
#include "renderer/opengl/texture_ogl.hpp"
//#include "renderer/vulkan/texture_impl_vlk.hpp"

namespace jng {

    Ref<Texture> Texture::create(const char* path)
	{
        JNG_PROFILE_FUNCTION();

        return makeRef<OpenGLTexture>(path);
	}

    Ref<Texture> Texture::create(uint32 width, uint32 height)
    {
        JNG_PROFILE_FUNCTION();

        return makeRef<OpenGLTexture>(width, height);
    }

} // namespace jng
