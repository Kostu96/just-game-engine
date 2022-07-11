/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/framebuffer.hpp"

#include "platform/windows/windows_base.hpp"

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

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
		void* getColorAttachmentHandle() override { return m_colorAttachmentView.Get(); }
	private:
		void recreate();

		Properties m_properties;
		const Direct3DGraphicsContext* m_graphicsContext;
		wrl::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		wrl::ComPtr<ID3D11ShaderResourceView> m_colorAttachmentView;
	};

} // namespace jng
