/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

	class Framebuffer
	{
	public:
		struct Properties {
			uint32 width;
			uint32 height;
			uint32 samples = 1;

			bool swapChainTarget = false;
		};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static Ref<Framebuffer> create(const Properties& properties);
		virtual ~Framebuffer() = default;
	};

} // namespace jng
