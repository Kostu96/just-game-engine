/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/framebuffer.hpp"

namespace jng {

	class Direct3DFramebuffer :
		public Framebuffer
	{
	public:
		explicit Direct3DFramebuffer(const Properties& properties);
		virtual ~Direct3DFramebuffer();

		void bind() const override;
		void unbind() const override;
	private:
		void recreate();

		Properties m_properties;
		uint32 m_ID;
		uint32 m_colorAttachmentID;
		uint32 m_depthAttachmentID;
	};

} // namespace jng
