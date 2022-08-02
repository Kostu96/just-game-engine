/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/framebuffer.hpp"

#include "platform/windows/windows_base.hpp"

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

namespace jng {

	class Direct3DGraphicsContext;

	namespace wrl = Microsoft::WRL;

	class Direct3DFramebuffer :
		public Framebuffer
	{
	public:
		explicit Direct3DFramebuffer(const Properties& properties);
		virtual ~Direct3DFramebuffer();

		void bind() const override;
		void unbind() const override;
		void resize(uint32 width, uint32 height) override;
		uint32 readPixel(uint32 /*colorAttachmentIndex*/, uint32 /*x*/, uint32 /*y*/) const override { return 0; }
		const std::vector<Ref<Texture>>& getColorAttachments() const override { return m_attachments; }
		const Properties& getProperties() const override { return m_properties; }
	private:
		void recreate();

		Properties m_properties;
		const Direct3DGraphicsContext* m_graphicsContext;
		wrl::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		wrl::ComPtr<ID3D11ShaderResourceView> m_colorAttachmentView;
		std::vector<Ref<Texture>> m_attachments;
	};

} // namespace jng
