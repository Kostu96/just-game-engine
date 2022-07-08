/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/opengl/shader_ogl.hpp"

#include "core/base_internal.hpp"
#include "core/engine.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.inl>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <filesystem>
#include <ccl/helper_functions.h>
#include <vector>

namespace jng {

	OpenGLShader::OpenGLShader(std::string_view vertexShaderFilename, std::string_view fragmentShaderFilename)
    {
		createCacheDirectoryIfNeeded();

		uint32 vs = compileShader(vertexShaderFilename.data(), Type::Vertex);
		uint32 fs = compileShader(fragmentShaderFilename.data(), Type::Fragment);
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

	std::filesystem::path OpenGLShader::getCacheDirectory() const
	{
		return Engine::get().getProperties().assetsDirectory / std::filesystem::path{ "cache/shaders/opengl" };
	}

	uint32 OpenGLShader::compileShader(const char* shaderFilename, Type type) const
	{
		std::vector<uint32> vulkanSpirvData = compileToVulkanSPIRV(shaderFilename, type);
		
		// Check for cached OpenGL SPIR-V
		std::filesystem::path cacheDirectory = getCacheDirectory();
		std::filesystem::path shaderFilePath = shaderFilename;
		std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.stem().string() + shaderTypeToCachedOGLFileExtension(type));
		
		bool success;
		std::vector<uint32> openglSpirvData;
		if (m_isCacheDirty) {
			spirv_cross::CompilerGLSL glslCompiler{ vulkanSpirvData };
			std::string openglCode = glslCompiler.compile();

			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
			auto openGLSpirv = compiler.CompileGlslToSpv(openglCode, static_cast<shaderc_shader_kind>(shaderTypeToShaderCKind(type)), shaderFilename, options);
			JNG_CORE_ASSERT(openGLSpirv.GetCompilationStatus() == shaderc_compilation_status_success, openGLSpirv.GetErrorMessage());
			openglSpirvData = std::vector<uint32>{ openGLSpirv.cbegin(), openGLSpirv.cend() };

			success = ccl::writeFile(cachedPath.generic_string().c_str(), reinterpret_cast<char*>(openglSpirvData.data()), openglSpirvData.size() * sizeof(uint32), true);
		}
		else {
			size_t size;
			success = ccl::readFile(cachedPath.generic_string().c_str(), nullptr, size, true);
			openglSpirvData.resize(size / sizeof(uint32));
			success = ccl::readFile(cachedPath.generic_string().c_str(), reinterpret_cast<char*>(openglSpirvData.data()), size, true);
		}

		uint32 id = glCreateShader(shaderTypeToOGLShaderType(type));
		glShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V, openglSpirvData.data(), static_cast<GLsizei>(openglSpirvData.size() * sizeof(uint32)));
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

	const char* OpenGLShader::shaderTypeToCachedOGLFileExtension(Type type)
	{
		switch (type)
		{
		case Type::Vertex:   return ".cached_ogl.vert";
		case Type::Fragment: return ".cached_ogl.frag";
		}
		JNG_CORE_ASSERT(false, "");
		return "";
	}

	uint32 OpenGLShader::shaderTypeToOGLShaderType(Type type)
	{
		switch (type)
		{
		case Type::Vertex:   return GL_VERTEX_SHADER;
		case Type::Fragment: return GL_FRAGMENT_SHADER;
		}
		JNG_CORE_ASSERT(false, "");
		return static_cast<uint32>(-1);
	}

} // namespace jng
