/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#define JNG_BIND_EVENT_FUNC(func) [this](auto&& ...args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

namespace jng {

    class NonCopyable
    {
    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;
    private:
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };

    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> makeScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> makeRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;
    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;

    enum class RendererBackend {
        None = -1,
        OpenGL,
        Vulkan,
        Direct3D
    };

} // namespace jng

#include "jng/debug/assert.hpp"
#include "jng/debug/log.hpp"
