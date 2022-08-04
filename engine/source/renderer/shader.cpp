/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/shader.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/opengl/shader_ogl.hpp"

#if 0
#if defined(JNG_WINDOWS)
#include "renderer/direct3d/shader_d3d.hpp"
#endif
#endif

#include "utilities/file.hpp"

#include <shaderc/shaderc.hpp>
#include <filesystem>

namespace jng {
    
    Ref<Shader> Shader::create(std::string_view vertexShaderFilename, std::string_view fragmentShaderFilename)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
#if 0
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DShader>(vertexShaderFilename, fragmentShaderFilename);
#endif
#endif
        case RendererBackend::OpenGL: return makeRef<OpenGLShader>(vertexShaderFilename, fragmentShaderFilename);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

    std::vector<uint32> Shader::compileToVulkanSPIRV(const char* shaderFilename, Type type) const
    {
        std::filesystem::path shaderFilePath = shaderFilename;
        std::filesystem::path cacheDirectory = getCacheDirectory();

        size_t size;
        bool success = readFile(shaderFilename, nullptr, size, true);
        JNG_CORE_ASSERT(success, std::string{ "Cannot open filename: " } + shaderFilename);
        char* shaderSource = new char[size + 1];
        success = readFile(shaderFilename, shaderSource, size, true);
        shaderSource[size] = 0;

        std::string shaderSourceString{ shaderSource };
        size_t shaderSourceHash = std::hash<std::string>{}(shaderSourceString);

        std::filesystem::path hashPath = cacheDirectory / (shaderFilePath.stem().string() + shaderTypeToHashFileExtension(type));

        m_isCacheDirty = false;
        success = readFile(hashPath.string().c_str(), nullptr, size, true);
        if (success) {
            static_assert(sizeof(size_t) == 8);
            JNG_CORE_ASSERT(size == 8, "std::hash should always be 8 bytes!");
            size_t savedHash;
            success = readFile(hashPath.string().c_str(), reinterpret_cast<char*>(&savedHash), size, true);

            if (savedHash != shaderSourceHash) m_isCacheDirty = true;
        }
        else
            m_isCacheDirty = true;

        JNG_CORE_TRACE("Shader cache is {0}.", m_isCacheDirty ? "dirty" : "good");

        std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.stem().string() + shaderTypeToCachedVlkFileExtension(type));
        
        std::vector<uint32> vulkanSpirvData;
        if (m_isCacheDirty) {
            JNG_CORE_TRACE("Recompiling Vulkan SPIR-V...");

            shaderc::Compiler compiler;
            shaderc::CompileOptions options;
            options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
            options.SetOptimizationLevel(shaderc_optimization_level_performance);
            auto vulkanSpirv = compiler.CompileGlslToSpv(shaderSource, static_cast<shaderc_shader_kind>(shaderTypeToShaderCKind(type)), shaderFilename, options);
            JNG_CORE_ASSERT(vulkanSpirv.GetCompilationStatus() == shaderc_compilation_status_success, vulkanSpirv.GetErrorMessage());
            vulkanSpirvData = std::vector<uint32>{ vulkanSpirv.cbegin(), vulkanSpirv.cend() };

            writeFile(hashPath.string().c_str(), reinterpret_cast<char*>(&shaderSourceHash), sizeof(size_t), true);
            writeFile(cachedPath.string().c_str(), reinterpret_cast<char*>(vulkanSpirvData.data()), vulkanSpirvData.size() * sizeof(uint32), true);
        }
        else {
            JNG_CORE_TRACE("Loading Vulkan SPIR-V from cache: {0}",
                cachedPath.string().c_str());

            success = readFile(cachedPath.string().c_str(), nullptr, size, true);
            vulkanSpirvData.resize(size / sizeof(uint32));
            success = readFile(cachedPath.string().c_str(), reinterpret_cast<char*>(vulkanSpirvData.data()), size, true);
        }

        delete[] shaderSource;

        // TODO: Reflect

        return vulkanSpirvData;
    }

    void Shader::createCacheDirectoryIfNeeded() const
    {
        auto cacheDirectory = getCacheDirectory();
        if (!std::filesystem::exists(cacheDirectory))
            std::filesystem::create_directories(cacheDirectory);
    }

    const char* Shader::shaderTypeToHashFileExtension(Type type)
    {
        switch (type)
        {
        case Type::Vertex:    return ".hash.vert";
        case Type::Fragment:  return ".hash.frag";
        }
        JNG_CORE_ASSERT(false, "");
        return "";
    }

    const char* Shader::shaderTypeToCachedVlkFileExtension(Type type)
    {
        switch (type)
        {
        case Type::Vertex:    return ".cached_vlk.vert";
        case Type::Fragment:  return ".cached_vlk.frag";
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
