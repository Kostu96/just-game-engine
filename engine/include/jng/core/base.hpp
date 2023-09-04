/*
 * Copyright (C) 2021-2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <cstdint>
#include <memory>

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
    template<typename T, typename ...Args>
    constexpr Scope<T> makeScope(Args&& ...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ...Args>
    constexpr Ref<T> makeRef(Args&& ...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    using u8  = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;
    using s8  = int8_t;
    using s16 = int16_t;
    using s32 = int32_t;
    using s64 = int64_t;
    using f32 = float;
    using f64 = double;

} // namespace jng

#include "jng/debug/assert.hpp"
#include "jng/debug/log.hpp"
