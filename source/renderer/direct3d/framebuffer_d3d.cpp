/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/direct3d/framebuffer_d3d.hpp"

#include "core/base_internal.hpp"

namespace jng {

	Direct3DFramebuffer::Direct3DFramebuffer(const Properties& properties) :
		m_properties{ properties }
	{
		recreate();
	}

	Direct3DFramebuffer::~Direct3DFramebuffer() = default;

	void Direct3DFramebuffer::bind() const
	{
		
	}

	void Direct3DFramebuffer::unbind() const
	{
		
	}

	void Direct3DFramebuffer::recreate()
	{
		
	}

} // namespace jng
