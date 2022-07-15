/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

    class Renderer :
        public NonCopyable
    {
    public:
        static void init();
        static void shutdown();
    private:
    };

} // namespace jng
