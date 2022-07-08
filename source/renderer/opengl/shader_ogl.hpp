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
        OpenGLShader(std::string_view vertexShaderFilename, std::string_view fragmentShaderFilename);
        virtual ~OpenGLShader();

        void bind() const override;
        void unbind() const override;

        void set(const char* name, int value) const override;
        void set(const char* name, const int* value, uint32 count) const override;
        void set(const char* name, const glm::vec3& value) const override;
        void set(const char* name, const glm::vec4& value) const override;
        void set(const char* name, const glm::mat4& value) const override;
    protected:
        std::filesystem::path getCacheDirectory() const override;
    private:
        uint32 compileShader(const char* shaderFilename, Type type) const;

        static const char* shaderTypeToCachedOGLFileExtension(Type type);
        static uint32 shaderTypeToOGLShaderType(Type type);

        uint32 m_id;
    };

} // namespace jng
