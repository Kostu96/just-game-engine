/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/shader.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

#include "renderer/direct3d/shader_impl_d3d.hpp"
#if defined(JNG_RENDER_API_OPENGL)
    #include "renderer/opengl/shader_impl_ogl.hpp"
#elif defined(JNG_RENDER_API_VULKAN)
    #include "renderer/vulkan/shader_impl_vlk.hpp"
#endif

namespace jng {
    
    void Shader::bind() const
    {
        m_implementation->bind();
    }

    void Shader::unbind() const
    {
        m_implementation->unbind();
    }

    void Shader::set(const char* name, int value) const
    {
        m_implementation->set(name, value);
    }

    void Shader::set(const char* name, const int* value, uint32 count) const
    {
        m_implementation->set(name, value, count);
    }

    void Shader::set(const char* name, const glm::vec3& value) const
    {
        m_implementation->set(name, value);
    }

    void Shader::set(const char* name, const glm::vec4& value) const
    {
        m_implementation->set(name, value);
    }

    void Shader::set(const char* name, const glm::mat4& value) const
    {
        m_implementation->set(name, value);
    }

	Ref<Shader> Shader::create(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc)
	{
        JNG_PROFILE_FUNCTION();

        return makeRef<Shader>(makeScope<ShaderImpl>(vertexShaderSrc, fragmentShaderSrc));
	}

    Shader::Shader(Scope<ShaderImpl>&& implementation) :
        m_implementation{ std::move(implementation) } {}

    Shader::~Shader() = default;

} // namespace jng
