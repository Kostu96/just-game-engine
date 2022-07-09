/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/framebuffer.hpp"

namespace jng {

	class OpenGLFramebuffer :
		public Framebuffer
	{
	public:
		explicit OpenGLFramebuffer(const Properties& properties);
		virtual ~OpenGLFramebuffer();

		void bind() const override;
		void unbind() const override;

		void* getColorAttachmentHandle() override { return reinterpret_cast<void*>(&m_colorAttachmentID); }
	private:
		void recreate();

		Properties m_properties;
		uint32 m_ID;
		uint32 m_colorAttachmentID;
		uint32 m_depthAttachmentID;
	};

} // namespace jng
