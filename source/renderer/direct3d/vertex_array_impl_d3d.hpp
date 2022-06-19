/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/vertex_array.hpp"

#include "platform/windows/windows_base.hpp"
#include "platform/graphics_context_d3d.hpp"

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

struct ID3D11InputLayout;

namespace jng {

    class VertexArrayImpl
    {
    public:
        VertexArrayImpl(const Ref<VertexBuffer>& vbo, const VertexLayout& layout, const Ref<Shader>& shader);
        ~VertexArrayImpl();

        void bind() const;
        void unbind() const;

        const Ref<VertexBuffer>& getVertexBuffer() const { return m_VBO; }
        void setIndexBuffer(const Ref<IndexBuffer>& ibo);
        const Ref<IndexBuffer>& getIndexBuffer() const { return m_IBO; }
    private:
        const Direct3DGraphicsContext& m_graphicsContext;
        Ref<VertexBuffer> m_VBO;
        Ref<IndexBuffer> m_IBO;
        wrl::ComPtr<ID3D11InputLayout> m_nativeLayout;
    };

} // namespace jng
