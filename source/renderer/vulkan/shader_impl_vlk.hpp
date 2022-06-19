/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/shader.hpp"

namespace k2d {

    class ShaderImpl
    {
    public:
        ShaderImpl(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc);
        ~ShaderImpl();

        void bind() const;
        void unbind() const;

        void set(const char* name, int value) const;
        void set(const char* name, const int* value, uint32 count) const;
        void set(const char* name, const glm::vec3& value) const;
        void set(const char* name, const glm::vec4& value) const;
        void set(const char* name, const glm::mat4& value) const;
    private:
        uint32 compileShader(const char* shaderSource, uint32 shaderType) const;

        uint32 m_id;
    };

} // namespace k2d
