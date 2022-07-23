/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

    class GUID
    {
    public:
        GUID();
        explicit GUID(uint64 guid) : m_GUID{ guid } {}
        GUID(const GUID&) = default;

        operator uint64() const { return m_GUID; }
    private:
        uint64 m_GUID;
    };

} // namespace jng
