/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/opengl/shader_ogl.hpp"

#include "core/base_internal.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.inl>

namespace jng {

	OpenGLShader::OpenGLShader(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc)
    {
		uint32 vs = compileShader(vertexShaderSrc.data(), GL_VERTEX_SHADER);
		uint32 fs = compileShader(fragmentShaderSrc.data(), GL_FRAGMENT_SHADER);
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
		}

		glDeleteShader(vs);
		glDeleteShader(fs);
    }

	OpenGLShader::~OpenGLShader()
    {
		glDeleteProgram(m_id);
    }

    void OpenGLShader::bind() const
    {
		glUseProgram(m_id);
    }

	void OpenGLShader::unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::set(const char* name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_id, name), value);
	}

	void OpenGLShader::set(const char* name, const int* value, uint32 count) const
	{
		glUniform1iv(glGetUniformLocation(m_id, name), static_cast<int>(count), value);
	}

	void OpenGLShader::set(const char* name, const glm::vec3& value) const
    {
		glUniform3fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
    }

	void OpenGLShader::set(const char* name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
	}

    void OpenGLShader::set(const char* name, const glm::mat4& value) const
    {
		glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(value));
    }

	uint32 OpenGLShader::compileShader(const char* shaderSource, uint32 shaderType) const
	{
		int success;
		uint32 id = glCreateShader(shaderType);

		glShaderSource(id, 1, &shaderSource, nullptr);
		glCompileShader(id);
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
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
