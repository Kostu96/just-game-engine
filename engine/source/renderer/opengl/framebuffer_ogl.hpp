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
		void resize(uint32 width, uint32 height) override;
		uint32 readPixel(uint32 colorAttachmentIndex, uint32 x, uint32 y) const override;
		void clearAttachment(uint32 attachmentIndex, int value) const override;
		void clearAttachment(uint32 attachmentIndex, float value) const override;
		
		const std::vector<Ref<Texture>>& getColorAttachments() const override { return m_attachments; }
		const Properties& getProperties() const override { return m_properties; }
	private:
		void recreate();

		Properties m_properties;
		uint32 m_ID;
		std::vector<Ref<Texture>> m_attachments;
		uint32 m_colorAttachmentID;
		uint32 m_depthAttachmentID;
	};

} // namespace jng