/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "debug/profiling.hpp"

#if JNG_PROFILING_ENABLED

#include <sstream>

namespace jng {

    void Profiler::beginSession(const std::string& filepath)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_output.open(filepath);
        writeHeader();
    }

    void Profiler::endSession()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        writeFooter();
        m_output.close();
    }

    void Profiler::addProfile(const Result& result)
    {
        std::stringstream json;
        json << ",{"
            << "\"cat\":\"function\","
            << "\"dur\":" << result.end - result.start << ','
            << "\"name\":\"" << result.name << "\","
            << "\"ph\":\"X\","
            << "\"pid\":0,"
            << "\"tid\":" << std::this_thread::get_id() << ','
            << "\"ts\":" << result.start
            << '}';

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_output << json.str();
            m_output.flush();
        }
    }

    void Profiler::writeHeader()
    {
        m_output << "[{}";
        m_output.flush();
    }

    void Profiler::writeFooter()
    {
        m_output << ']';
        m_output.flush();
    }

    Profiler Profiler::s_instance;

} // namespace jng

#endif
