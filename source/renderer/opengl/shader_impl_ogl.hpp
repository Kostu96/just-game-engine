/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/shader.hpp"

namespace jng {

    class OpenGLShader
    {
    public:
        OpenGLShader(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc);
        ~OpenGLShader();

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

} // namespace jng
