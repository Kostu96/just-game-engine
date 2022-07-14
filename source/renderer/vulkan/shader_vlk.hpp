/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/shader.hpp"

namespace jng {

    class VulkanShader :
        public Shader
    {
    public:
        VulkanShader(std::string_view vertexShaderFilename, std::string_view fragmentShaderFilename);
        ~VulkanShader();

        void bind() const;
        void unbind() const;
    protected:
        std::filesystem::path getCacheDirectory() const override;
    private:
        uint32 compileShader(const char* shaderSource, uint32 shaderType) const;

        uint32 m_id;
    };

} // namespace jng
