/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

#include <glm/glm.hpp>
#include <string_view>

namespace jng {

    class ShaderImpl;

    class Shader
    {
    public:
        void bind() const;
        void unbind() const;

        void set(const char* name, int value) const;
        void set(const char* name, const int* value, uint32 count) const;
        void set(const char* name, const glm::vec3& value) const;
        void set(const char* name, const glm::vec4& value) const;
        void set(const char* name, const glm::mat4& value) const;

        ShaderImpl* getImplementation() const { return m_implementation.get(); }

        static Ref<Shader> create(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc);
        Shader(Scope<ShaderImpl>&& implementation);
        ~Shader();
    private:
        Scope<ShaderImpl> m_implementation;
    };

} // namespace jng
