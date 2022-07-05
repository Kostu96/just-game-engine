/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/texture.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

#include "renderer/opengl/texture_ogl.hpp"
//#include "renderer/vulkan/texture_impl_vlk.hpp"

#if defined(JNG_WINDOWS)
#include "renderer/direct3d/texture_d3d.hpp"
#endif

namespace jng {

    Ref<Texture> Texture::create(const char* path)
	{
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DTexture>(path);
#endif
        case RendererBackend::OpenGL: return makeRef<OpenGLTexture>(path);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
	}

    Ref<Texture> Texture::create(uint32 width, uint32 height)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DTexture>(width, height);
#endif
        case RendererBackend::OpenGL: return makeRef<OpenGLTexture>(width, height);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

} // namespace jng
