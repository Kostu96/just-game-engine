/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/shader.hpp"

namespace jng {

    class OpenGLShader :
        public Shader
    {
    public:
        OpenGLShader(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc);
        virtual ~OpenGLShader();

        void bind() const override;
        void unbind() const override;

        void set(const char* name, int value) const override;
        void set(const char* name, const int* value, uint32 count) const override;
        void set(const char* name, const glm::vec3& value) const override;
        void set(const char* name, const glm::vec4& value) const override;
        void set(const char* name, const glm::mat4& value) const override;
    private:
        uint32 compileShader(const char* shaderSource, uint32 shaderType) const;

        uint32 m_id;
    };

} // namespace jng
