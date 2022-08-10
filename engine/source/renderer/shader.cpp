/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/shader.hpp"

#include "core/base_internal.hpp"
#include "core/engine.hpp"

#include "utilities/file.hpp"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <vector>

namespace jng {

    static const char* shaderTypeToCachedOGLFileExtension(Shader::Type type)
    {
        switch (type)
        {
        case Shader::Type::Vertex:   return ".vert.ogl_cache";
        case Shader::Type::Fragment: return ".frag.ogl_cache";
        }
        JNG_CORE_ASSERT(false, "");
        return "";
    }

    std::filesystem::path Shader::getCacheDirectory() const
    {
        return Engine::get().getProperties().assetsDirectory / std::filesystem::path{ "cache/shaders" };
    }

    void Shader::createCacheDirectoryIfNeeded() const
    {
        auto cacheDirectory = getCacheDirectory();
        if (!std::filesystem::exists(cacheDirectory))
            std::filesystem::create_directories(cacheDirectory);
    }

    std::vector<uint32> Shader::compileToSPIRV(const std::filesystem::path& filename, Type type) const
    {
        std::string filenameStr = filename.string();
        std::string shaderFileStem = filename.stem().string();
        std::filesystem::path cacheDirectory = getCacheDirectory();

        size_t size;
        bool success = readFile(filenameStr.c_str(), nullptr, size, true);
        JNG_CORE_ASSERT(success, "Cannot open filename: " + filenameStr);
        char* shaderSource = new char[size + 1];
        success = readFile(filenameStr.c_str(), shaderSource, size, true);
        shaderSource[size] = 0;

        std::string shaderSourceString{ shaderSource };
        size_t shaderSourceHash = std::hash<std::string>{}(shaderSourceString);

        std::filesystem::path hashPath = cacheDirectory / (shaderFileStem + shaderTypeToHashFileExtension(type));
        std::string hashPathStr = hashPath.string();

        m_isCacheDirty = false;
        success = readFile(hashPathStr.c_str(), nullptr, size, true);
        if (success) {
            static_assert(sizeof(size_t) == 8);
            JNG_CORE_ASSERT(size == 8, "std::hash should always be 8 bytes!");
            size_t savedHash;
            success = readFile(hashPathStr.c_str(), reinterpret_cast<char*>(&savedHash), size, true);

            if (savedHash != shaderSourceHash) m_isCacheDirty = true;
        }
        else
            m_isCacheDirty = true;

        std::filesystem::path vlkCachedPath = cacheDirectory / (shaderFileStem + shaderTypeToCachedVlkFileExtension(type));
        std::string vlkCachedPathStr = vlkCachedPath.string();
        
        std::vector<uint32> vulkanSpirvData;
        if (m_isCacheDirty) {
            shaderc::Compiler compiler;
            shaderc::CompileOptions options;
            options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
            options.SetOptimizationLevel(shaderc_optimization_level_performance);
            auto vulkanSpirv = compiler.CompileGlslToSpv(shaderSource, static_cast<shaderc_shader_kind>(shaderTypeToShaderCKind(type)), filenameStr.c_str(), options);
            JNG_CORE_ASSERT(vulkanSpirv.GetCompilationStatus() == shaderc_compilation_status_success, vulkanSpirv.GetErrorMessage());
            vulkanSpirvData = std::vector<uint32>{ vulkanSpirv.cbegin(), vulkanSpirv.cend() };

            writeFile(hashPathStr.c_str(), reinterpret_cast<char*>(&shaderSourceHash), sizeof(size_t), true);
            writeFile(vlkCachedPathStr.c_str(), reinterpret_cast<char*>(vulkanSpirvData.data()), vulkanSpirvData.size() * sizeof(uint32), true);
        }
        else {
            success = readFile(vlkCachedPathStr.c_str(), nullptr, size, true);
            vulkanSpirvData.resize(size / sizeof(uint32));
            success = readFile(vlkCachedPathStr.c_str(), reinterpret_cast<char*>(vulkanSpirvData.data()), size, true);
        }

        delete[] shaderSource;

        // TODO: Reflect

        // Check for cached OpenGL SPIR-V
        std::filesystem::path oglCachedPath = cacheDirectory / (shaderFileStem + shaderTypeToCachedOGLFileExtension(type));
        std::string oglCachedPathStr = oglCachedPath.string();

        std::vector<uint32> openglSpirvData;
        if (m_isCacheDirty) {
            spirv_cross::CompilerGLSL glslCompiler{ vulkanSpirvData };
            std::string openglCode = glslCompiler.compile();

            shaderc::Compiler compiler;
            shaderc::CompileOptions options;
            options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
            options.SetOptimizationLevel(shaderc_optimization_level_performance);
            auto openGLSpirv = compiler.CompileGlslToSpv(openglCode, static_cast<shaderc_shader_kind>(shaderTypeToShaderCKind(type)), filenameStr.c_str(), options);
            JNG_CORE_ASSERT(openGLSpirv.GetCompilationStatus() == shaderc_compilation_status_success, openGLSpirv.GetErrorMessage());
            openglSpirvData = std::vector<uint32>{ openGLSpirv.cbegin(), openGLSpirv.cend() };

            success = writeFile(oglCachedPathStr.c_str(), reinterpret_cast<char*>(openglSpirvData.data()), openglSpirvData.size() * sizeof(uint32), true);
        }
        else {
            success = readFile(oglCachedPathStr.c_str(), nullptr, size, true);
            openglSpirvData.resize(size / sizeof(uint32));
            success = readFile(oglCachedPathStr.c_str(), reinterpret_cast<char*>(openglSpirvData.data()), size, true);
        }

        return openglSpirvData;
    }

    const char* Shader::shaderTypeToHashFileExtension(Type type)
    {
        switch (type)
        {
        case Type::Vertex:    return ".vert.hash";
        case Type::Fragment:  return ".frag.hash";
        }
        JNG_CORE_ASSERT(false, "");
        return "";
    }

    const char* Shader::shaderTypeToCachedVlkFileExtension(Type type)
    {
        switch (type)
        {
        case Type::Vertex:    return ".vert.vlk_cache";
        case Type::Fragment:  return ".frag.vlk_cache";
        }
        JNG_CORE_ASSERT(false, "");
        return "";
    }

    uint32 Shader::shaderTypeToShaderCKind(Type type)
    {
        switch (type)
        {
        case Type::Vertex:  return shaderc_glsl_vertex_shader;
        case Type::Fragment:  return shaderc_glsl_fragment_shader;
        }
        JNG_CORE_ASSERT(false, "");
        return static_cast<uint32>(-1);
    }

} // namespace jng
