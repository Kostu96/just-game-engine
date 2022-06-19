/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

#include <glm/glm.hpp>
#include <string_view>

namespace jng {

    class Shader
    {
    public:
        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void set(const char* name, int value) const = 0;
        virtual void set(const char* name, const int* value, uint32 count) const = 0;
        virtual void set(const char* name, const glm::vec3& value) const = 0;
        virtual void set(const char* name, const glm::vec4& value) const = 0;
        virtual void set(const char* name, const glm::mat4& value) const = 0;

        static Ref<Shader> create(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc);
        virtual ~Shader() = default;
    };

} // namespace jng
