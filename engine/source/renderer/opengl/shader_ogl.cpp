/*
 * Copyright (C) 2021-2023 Konstanty Misiak
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
        JNG_CORE_ASSERT(false, "This should never be triggered");
        return static_cast<u32>(-1);
    }

    static const char* attributeTypeToStr(GLenum type)
    {
        switch (type)
        {
        case GL_FLOAT:             return "FLOAT";
        case GL_FLOAT_VEC2:        return "FLOAT_VEC2";
        case GL_FLOAT_VEC3:        return "FLOAT_VEC3";
        case GL_FLOAT_VEC4:        return "FLOAT_VEC4";
        case GL_FLOAT_MAT2:        return "FLOAT_MAT2";
        case GL_FLOAT_MAT3:        return "FLOAT_MAT3";
        case GL_FLOAT_MAT4:        return "FLOAT_MAT4";
        case GL_FLOAT_MAT2x3:      return "FLOAT_MAT2x3";    
        case GL_FLOAT_MAT2x4:      return "FLOAT_MAT2x4";    
        case GL_FLOAT_MAT3x2:      return "FLOAT_MAT3x2";    
        case GL_FLOAT_MAT3x4:      return "FLOAT_MAT3x4";    
        case GL_FLOAT_MAT4x2:      return "FLOAT_MAT4x2";    
        case GL_FLOAT_MAT4x3:      return "FLOAT_MAT4x3";    
        case GL_INT:               return "INT";
        case GL_INT_VEC2:          return "INT_VEC2";        
        case GL_INT_VEC3:          return "INT_VEC3";        
        case GL_INT_VEC4:          return "INT_VEC4";        
        case GL_UNSIGNED_INT:      return "UNSIGNED_INT";
        case GL_UNSIGNED_INT_VEC2: return "UNSIGNED_INT_VEC2";
        case GL_UNSIGNED_INT_VEC3: return "UNSIGNED_INT_VEC3";
        case GL_UNSIGNED_INT_VEC4: return "UNSIGNED_INT_VEC4";
        case GL_DOUBLE:            return "DOUBLE";
        case GL_DOUBLE_VEC2:       return "DOUBLE_VEC2";     
        case GL_DOUBLE_VEC3:       return "DOUBLE_VEC3";     
        case GL_DOUBLE_VEC4:       return "DOUBLE_VEC4";     
        case GL_DOUBLE_MAT2:       return "DOUBLE_MAT2";     
        case GL_DOUBLE_MAT3:       return "DOUBLE_MAT3";     
        case GL_DOUBLE_MAT4:       return "DOUBLE_MAT4";     
        case GL_DOUBLE_MAT2x3:     return "DOUBLE_MAT2x3";   
        case GL_DOUBLE_MAT2x4:     return "DOUBLE_MAT2x4";   
        case GL_DOUBLE_MAT3x2:     return "DOUBLE_MAT3x2";   
        case GL_DOUBLE_MAT3x4:     return "DOUBLE_MAT3x4";   
        case GL_DOUBLE_MAT4x2:     return "DOUBLE_MAT4x2";   
        case GL_DOUBLE_MAT4x3:     return "DOUBLE_MAT4x3";   
        }
        JNG_CORE_ASSERT(false, "This should never be triggered");
        return "";
    }

    Shader::Shader(const std::filesystem::path& vertexShaderFilename, const std::filesystem::path& fragmentShaderFilename)
    {
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

        GLint maxAttributeLength, attributeCount;
        glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &attributeCount);
        glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeLength);
        GLchar* attrName = new GLchar[maxAttributeLength + 1];
        for (GLint i = 0; i < attributeCount; i++)
        {
            GLint size;
            GLenum type;
            glGetActiveAttrib(m_id, i, maxAttributeLength, nullptr, &size, &type, attrName);
            JNG_CORE_TRACE("  Attribute {}: {} : {}", i, attrName, attributeTypeToStr(type));
        }
        delete[] attrName;

        GLint maxUniformLength, uniformCount;
        glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &uniformCount);
        glGetProgramiv(m_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength);
        GLchar* uniformName = new GLchar[maxUniformLength + 1];
        for (GLint i = 0; i < uniformCount; i++)
        {
            GLint size;
            GLenum type;
            glGetActiveUniform(m_id, i, maxUniformLength, nullptr, &size, &type, uniformName);
            JNG_CORE_TRACE("  Uniform {}: {}", i, uniformName);
        }
        delete[] uniformName;

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

        std::string filenameStr = filename.string();
        size_t size;
        bool success = readFile(filenameStr.c_str(), nullptr, size, true);
        JNG_CORE_ASSERT(success, "Cannot open filename: " + filenameStr);
        char* shaderSource = new char[size + 1];
        success = readFile(filenameStr.c_str(), shaderSource, size, true);
        shaderSource[size] = 0;

        u32 id = glCreateShader(shaderTypeToOGLShaderType(type));
        glShaderSource(id, 1, &shaderSource, nullptr);
        glCompileShader(id);
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
