/*
 * Copyright (C) 2020-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifdef JNG_DEBUG

#include "jng/core/base.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <memory>

namespace jng {
    
    class Log :
        public ccl::NonCopyable
    {
    public:
        static void init();

        inline static spdlog::logger* getCoreLogger() { return m_coreLogger.get(); }
        inline static spdlog::logger* getUserLogger() { return m_userLogger.get(); }
    private:
        Log() = delete;
        Log(const Log&) = delete;

        static std::shared_ptr<spdlog::logger> m_coreLogger;
        static std::shared_ptr<spdlog::logger> m_userLogger;
    };

} // namespace jng

#define JNG_CORE_TRACE(...) Log::getCoreLogger()->trace(__VA_ARGS__)
#define JNG_CORE_INFO(...)  Log::getCoreLogger()->info(__VA_ARGS__)
#define JNG_CORE_WARN(...)  Log::getCoreLogger()->warn(__VA_ARGS__)
#define JNG_CORE_ERROR(...) Log::getCoreLogger()->error(__VA_ARGS__)
#define JNG_CORE_FATAL(...) Log::getCoreLogger()->critical(__VA_ARGS__)

#define JNG_USER_TRACE(...) jng::Log::getUserLogger()->trace(__VA_ARGS__)
#define JNG_USER_INFO(...)  jng::Log::getUserLogger()->info(__VA_ARGS__)
#define JNG_USER_WARN(...)  jng::Log::getUserLogger()->warn(__VA_ARGS__)
#define JNG_USER_ERROR(...) jng::Log::getUserLogger()->error(__VA_ARGS__)
#define JNG_USER_FATAL(...) jng::Log::getUserLogger()->critical(__VA_ARGS__)

#else

#define JNG_CORE_TRACE(...)
#define JNG_CORE_INFO(...)
#define JNG_CORE_WARN(...)
#define JNG_CORE_ERROR(...)
#define JNG_CORE_FATAL(...)

#define JNG_USER_TRACE(...)
#define JNG_USER_INFO(...)
#define JNG_USER_WARN(...)
#define JNG_USER_ERROR(...)
#define JNG_USER_FATAL(...)

#endif
