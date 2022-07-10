/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/vertex_array.hpp"

#include "platform/windows/windows_base.hpp"
#include "platform/windows/graphics_context_d3d.hpp"

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

struct ID3D11InputLayout;

namespace jng {

    class Direct3DVertexArray :
        public VertexArray
    {
    public:
        Direct3DVertexArray(const Ref<VertexBuffer>& vbo, const VertexLayout& layout, const Ref<Shader>& shader);
        ~Direct3DVertexArray();

        void bind() const override;
        void unbind() const override;

        const Ref<VertexBuffer>& getVertexBuffer() const override { return m_VBO; }
        void setIndexBuffer(const Ref<IndexBuffer>& ibo) override { m_IBO = ibo; }
        const Ref<IndexBuffer>& getIndexBuffer() const override { return m_IBO; }
    private:
        const Direct3DGraphicsContext* m_graphicsContext;
        Ref<VertexBuffer> m_VBO;
        Ref<IndexBuffer> m_IBO;
        wrl::ComPtr<ID3D11InputLayout> m_nativeLayout;
        VertexLayout m_layout;
    };

} // namespace jng
