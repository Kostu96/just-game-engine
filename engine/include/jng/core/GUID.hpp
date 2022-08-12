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
        explicit GUID(u64 guid) : m_GUID{ guid } {}
        GUID(const GUID&) = default;

        operator u64() const { return m_GUID; }
    private:
        u64 m_GUID;
    };

} // namespace jng
