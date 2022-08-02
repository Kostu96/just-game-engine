/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"
#include "jng/platform/key_codes.hpp"
#include "jng/platform/mouse_codes.hpp"

#include <glm/glm.hpp>

namespace jng {

    class Input
    {
    public:
        static bool isKeyPressed(Key::Code key);
        static bool isMouseButtonPressed(Mouse::Code button);
        static glm::vec2 getMousePosition();
    private:
        Input() = default;
        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete;
    };

} // namespace jng
