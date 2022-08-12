/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "core/GUID.hpp"

#include <random>

namespace jng {

    static std::random_device s_randomDevice;
    static std::mt19937_64 s_randomEngine{ s_randomDevice() };
    static std::uniform_int_distribution<u64> s_randomDistribution;

    GUID::GUID() :
        m_GUID{ s_randomDistribution(s_randomEngine) }
    {}

} // namespace jng
