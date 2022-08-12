/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/shader.hpp"

#include "core/base_internal.hpp"
#include "utilities/file.hpp"

#include <glad/gl.h>

namespace jng {

    static u32 shaderTypeToOGLShaderType(Shader::Type type)
    {
        switch (type)
        {
        case Shader::Type::Vertex:   return GL_VERTEX_SHADER;
        case Shader::Type::Fragment: return GL_FRAGMENT_SHADER;
        }
        JNG_CORE_ASSERT(false, "");
        return static_cast<u32>(-1);
    }

    Shader::Shader(const std::filesystem::path& vertexShaderFilename, const std::filesystem::path& fragmentShaderFilename)
    {
        createCacheDirectoryIfNeeded();

        u32 vs = compileShader(vertexShaderFilename, Type::Vertex);
        u32 fs = compileShader(fragmentShaderFilename, Type::Fragment);
        m_id = glCreateProgram();
        glAttachShader(m_id, vs);
        glAttachShader(m_id, fs);

        glLinkProgram(m_id);
        int success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success) {
            int maxLength = 0;
            glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);
            char* infoLog = new char[maxLength];
            glGetProgramInfoLog(m_id, maxLength, nullptr, infoLog);
            JNG_CORE_ERROR("Shader program linking failed! {0}", infoLog);
            glDeleteProgram(m_id);
        }

        glDetachShader(m_id, vs);
        glDetachShader(m_id, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_id);
    }

    void Shader::bind() const
    {
        glUseProgram(m_id);
    }

    void Shader::unbind() const
    {
        glUseProgram(0);
    }

    u32 Shader::compileShader(const std::filesystem::path& filename, Type type) const
    {
        JNG_CORE_TRACE("Compiling shader: {0}", filename);

        std::vector<u32> oglSpirvData = compileToSPIRV(filename, type);

        u32 id = glCreateShader(shaderTypeToOGLShaderType(type));
        glShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V, oglSpirvData.data(), static_cast<GLsizei>(oglSpirvData.size() * sizeof(u32)));
        glSpecializeShader(id, "main", 0, nullptr, nullptr);
        int ret;
        glGetShaderiv(id, GL_COMPILE_STATUS, &ret);
        if (!ret) {
            int maxLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
            char* infoLog = new char[maxLength];
            glGetShaderInfoLog(id, maxLength, nullptr, infoLog);
            JNG_CORE_ERROR("Shader compilation failed!\n{0}", infoLog);
            delete[] infoLog;
        };

        return id;
    }

} // namespace jng
