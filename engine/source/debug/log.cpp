/*
 * Copyright (C) 2020-2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#ifdef JNG_DEBUG

#include "debug/log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace jng {

    std::shared_ptr<spdlog::logger> Log::s_coreLogger;
    std::shared_ptr<spdlog::logger> Log::s_userLogger;

    static struct LogInitializer
    {
        LogInitializer() {
            spdlog::set_pattern("%^[%T][%n] %v%$");
            spdlog::set_level(spdlog::level::trace);
            Log::s_coreLogger = spdlog::stdout_color_mt("JNG");
            Log::s_userLogger = spdlog::stdout_color_mt("APP");
        }
    }
    globalLogInitializer;

} // namespace jng

#endif
