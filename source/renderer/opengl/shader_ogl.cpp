/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/opengl/shader_ogl.hpp"

#include "core/base_internal.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.inl>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace jng {

	static shaderc_shader_kind GLShaderTypeToShaderC(GLenum stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
		case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		}
		JNG_CORE_ASSERT(false, "");
		return static_cast<shaderc_shader_kind>(0);
	}

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
			glDeleteProgram(m_id);
		}

		glDetachShader(m_id, vs);
		glDetachShader(m_id, fs);
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
		shaderc::Compiler compiler;
		shaderc::CompileOptions options1;
		options1.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		options1.SetOptimizationLevel(shaderc_optimization_level_performance);

		auto vulkanSpirv = compiler.CompileGlslToSpv(shaderSource, GLShaderTypeToShaderC(shaderType), "filename", options1);
		JNG_CORE_ASSERT(vulkanSpirv.GetCompilationStatus() == shaderc_compilation_status_success, vulkanSpirv.GetErrorMessage());

		// TODO: Reflect

		shaderc::CompileOptions options2;
		options2.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		options2.SetOptimizationLevel(shaderc_optimization_level_performance);

		spirv_cross::CompilerGLSL glslCompiler{ std::vector<uint32>{ vulkanSpirv.cbegin(), vulkanSpirv.cend() }};
		std::string openglCode = glslCompiler.compile();

		shaderc::SpvCompilationResult openGLSpirv = compiler.CompileGlslToSpv(openglCode, GLShaderTypeToShaderC(shaderType), "filename", options2);
		JNG_CORE_ASSERT(openGLSpirv.GetCompilationStatus() == shaderc_compilation_status_success, openGLSpirv.GetErrorMessage());

		std::vector<uint32> openGLSpirvAsVector{ openGLSpirv.cbegin(), openGLSpirv.cend() };

		uint32 id = glCreateShader(shaderType);
		glShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V, openGLSpirvAsVector.data(), static_cast<GLsizei>(openGLSpirvAsVector.size() * sizeof(uint32)));
		glSpecializeShader(id, "main", 0, nullptr, nullptr);
		int success;
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
