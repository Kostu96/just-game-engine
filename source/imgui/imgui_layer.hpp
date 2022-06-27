/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"

namespace jng
{
	class Window;

	class ImGuiLayer
	{
	public:
		static void init(const Window* window);
		static void newFrame();
		static void render();
	private:
		ImGuiLayer() = delete;
	};

} // jng
