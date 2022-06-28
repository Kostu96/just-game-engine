/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/renderer.hpp"

#include "renderer/renderer2d.hpp"

namespace jng {

	void Renderer::init()
	{
		Renderer2D::init();
	}

	void Renderer::shutdown()
	{
		Renderer2D::shutdown();
	}

} // namespace jng
