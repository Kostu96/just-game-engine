/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"
#include "debug/profiling.hpp"

namespace jng {

    constexpr float PHYSICS_SCALE = 30.f;
    constexpr float PHYSICS_TIMESTEP = 1.f / 60.f;
    constexpr int PHYSICS_VEL_ITERATIONS = 8;
    constexpr int PHYSICS_POS_ITERATIONS = 3;

    constexpr float PI_F = 3.14159265f;
    constexpr double PI_D = 3.141592653589793;

} // namespace jng
