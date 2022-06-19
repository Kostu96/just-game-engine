/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"
#include "platform/key_codes.hpp"

namespace jng {

    class Platform
    {
    public:
        // TODO: is separate class needed only for this?
        static uint32 getNativeKeyCode(Key::KeyCode key);
    private:
        Platform() = default;
        Platform(const Platform&) = delete;
        Platform& operator=(const Platform&) = delete;
    };

} // namespace jng
