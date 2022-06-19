/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/platform.hpp"

namespace jng {

    uint32 Platform::getNativeKeyCode(Key::KeyCode key)
    {
        // NOTE: codes are the same as in GLFW, but this can change
        return key;
    }

} // namespace jng
