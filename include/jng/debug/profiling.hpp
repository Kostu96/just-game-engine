/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#define JNG_PROFILING_ENABLED 0
#if JNG_PROFILING_ENABLED

#include <chrono>
#include <fstream>
#include <thread>
#include <mutex>

namespace k2d {

    class Profiler
    {
    public:
        struct Result {
            std::string name;
            long long start, end;
        };

        void beginSession(const std::string& filepath);
        void endSession();
        void addProfile(const Result& result);

        static Profiler& get() { return s_instance; }
    private:
        Profiler() = default;

        void writeHeader();
        void writeFooter();

        std::ofstream m_output;
        std::mutex m_mutex;

        static Profiler s_instance;
    };

    class ProfilingTimer
    {
    public:
        ProfilingTimer(const char* name) :
            m_name(name),
            m_startTS(std::chrono::steady_clock::now())
        {}

        ~ProfilingTimer()
        {
            auto endTS = std::chrono::steady_clock::now();

            Profiler::get().addProfile({
                m_name,
                std::chrono::time_point_cast<std::chrono::microseconds>(m_startTS).time_since_epoch().count(),
                std::chrono::time_point_cast<std::chrono::microseconds>(endTS).time_since_epoch().count()
            });
        }
    private:
        std::string m_name;
        std::chrono::time_point<std::chrono::steady_clock> m_startTS;
    };

    template<size_t SIZE>
    constexpr auto cleanupStringForProfiler(const char(&str)[SIZE])
    {
        constexpr size_t arrSize = 3;
        constexpr const char* remove[arrSize] = {
            "__cdecl ",
            "class ",
            "struct "
        };
        constexpr size_t removeSize[3] = { 9, 7, 8 };
        char result[SIZE];

        size_t dstIndex = 0;
        for (size_t srcIndex = 0; srcIndex < SIZE; ++srcIndex)
        {
            for (size_t i = 0; i < arrSize; ++i)
            {
                size_t matchIndex = 0;
                while (matchIndex < removeSize[i] - 1 && srcIndex + matchIndex < SIZE - 1 && str[srcIndex + matchIndex] == remove[i][matchIndex])
                    matchIndex++;
                if (matchIndex == removeSize[i] - 1)
                    srcIndex += matchIndex;
            }

            result[dstIndex++] = str[srcIndex] == '"' ? '\'' : str[srcIndex];
        }
        return std::string(result);
}

} // namespace k2d

#if defined(__FUNCSIG__)
#define __K2D_FUNCSIG __FUNCSIG__
#else
#define __K2D_FUNCSIG __PRETTY_FUNCTION__
#endif

#define K2D_PROFILE_BEGIN_SESSION(filepath) k2d::Profiler::get().beginSession(filepath)
#define K2D_PROFILE_END_SESSION() k2d::Profiler::get().endSession()
#define __K2D_CONCAT(x, y) x##y
#define __K2D_PROFILE_SCOPE(name, line) k2d::ProfilingTimer __K2D_CONCAT(timer, line)(cleanupStringForProfiler(name).c_str())
#define K2D_PROFILE_SCOPE(name) __K2D_PROFILE_SCOPE(name, __LINE__)
#define K2D_PROFILE_FUNCTION() K2D_PROFILE_SCOPE(__K2D_FUNCSIG)

#else

#define JNG_PROFILE_BEGIN_SESSION(filepath)
#define JNG_PROFILE_END_SESSION()
#define JNG_PROFILE_SCOPE(name)
#define JNG_PROFILE_FUNCTION()

#endif
