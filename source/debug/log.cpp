/*
 * Copyright (C) 2020-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#ifdef JNG_DEBUG

#include "debug/log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace jng {

    std::shared_ptr<spdlog::logger> Log::m_coreLogger;
    std::shared_ptr<spdlog::logger> Log::m_userLogger;

    void Log::init()
    {
        spdlog::set_pattern("%^[%T][%n] %v%$");
        spdlog::set_level(spdlog::level::trace);
        m_coreLogger = spdlog::stdout_color_mt("K2D");
        m_userLogger = spdlog::stdout_color_mt("APP");
    }

    struct LogInitializer
    {
        LogInitializer() {
            Log::init();
        }
    }
    globalLogInitializer;

} // namespace jng

#endif
