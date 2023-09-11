/*
 * Copyright (C) 2021-2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

    class Shader final
    {
    public:
        enum class Type {
            Vertex,
            Fragment
        };

        Shader(const std::filesystem::path& vertexShaderFilename, const std::filesystem::path& fragmentShaderFilename);
        ~Shader();

        void bind() const;
        void unbind() const;
    private:
        u32 compileShader(const std::filesystem::path& filename, Type type) const;

        u32 m_id;
    };

} // namespace jng
