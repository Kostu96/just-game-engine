/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {
	namespace Mouse {
		
		using MouseCode = uint8;

		constexpr MouseCode Left = 0;
		constexpr MouseCode Right = 1;
		constexpr MouseCode Middle = 2;
		constexpr MouseCode X1 = 3;
		constexpr MouseCode X2 = 4;

	} // namespace Mouse
} // namespace jng
