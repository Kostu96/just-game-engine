/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/shader.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

#include "renderer/opengl/shader_ogl.hpp"
//#include "renderer/vulkan/shader_vlk.hpp"

#if defined(JNG_WINDOWS)
#include "renderer/direct3d/shader_d3d.hpp"
#endif

namespace jng {
    
    Ref<Shader> Shader::create(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc)
	{
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DShader>(vertexShaderSrc, fragmentShaderSrc);
#endif
        case RendererBackend::OpenGL: return makeRef<OpenGLShader>(vertexShaderSrc, fragmentShaderSrc);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
	}

} // namespace jng
