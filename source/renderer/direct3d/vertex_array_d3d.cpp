/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/direct3d/vertex_array_d3d.hpp"

#include "core/base_internal.hpp"
#include "core/engine.hpp"
#include "platform/window.hpp"
#include "platform/windows/error_checks_macros_win.hpp"
#include "renderer/buffers.hpp"
#include "renderer/direct3d/shader_d3d.hpp"

#include <d3d11.h>

namespace jng {

    static DXGI_FORMAT dataTypeToDXGIFormat(LayoutElement::DataType type)
    {
        switch (type)
        {
        case LayoutElement::DataType::Float:  return DXGI_FORMAT_R32_FLOAT;
        case LayoutElement::DataType::Float2: return DXGI_FORMAT_R32G32_FLOAT;
        case LayoutElement::DataType::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
        case LayoutElement::DataType::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case LayoutElement::DataType::Mat3:   return DXGI_FORMAT_UNKNOWN;
        case LayoutElement::DataType::Mat4:   return DXGI_FORMAT_UNKNOWN;
        case LayoutElement::DataType::Int:    return DXGI_FORMAT_UNKNOWN;
        case LayoutElement::DataType::Int2:   return DXGI_FORMAT_UNKNOWN;
        case LayoutElement::DataType::Int3:   return DXGI_FORMAT_UNKNOWN;
        case LayoutElement::DataType::Int4:   return DXGI_FORMAT_UNKNOWN;
        case LayoutElement::DataType::Bool:   return DXGI_FORMAT_UNKNOWN;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!")
            return DXGI_FORMAT_UNKNOWN;
    }

    /*

    static int dataTypeToCount(BufferElement::DataType type)
    {
        switch (type)
        {
        case BufferElement::DataType::Float:   return 1;
        case BufferElement::DataType::Float2:  return 2;
        case BufferElement::DataType::Float3:  return 3;
        case BufferElement::DataType::Float4:  return 4;
        case BufferElement::DataType::Mat3:    return 3 * 3;
        case BufferElement::DataType::Mat4:    return 4 * 4;
        case BufferElement::DataType::Int:     return 1;
        case BufferElement::DataType::Int2:    return 2;
        case BufferElement::DataType::Int3:    return 3;
        case BufferElement::DataType::Int4:    return 4;
        case BufferElement::DataType::Bool:    return 1;
        }

        K2D_CORE_ASSERT(false, "This should never be triggered!")
            return 0;
    }*/

    Direct3DVertexArray::Direct3DVertexArray(const Ref<VertexBuffer>& vbo, const VertexLayout& layout, const Ref<Shader>& shader) :
        m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) }
    {
        m_VBO = vbo;
        JNG_CORE_ASSERT(!layout.getElements().empty(), "Vertex buffer layout is empty!");

        HRESULT hr;
        const auto& device = m_graphicsContext->getNativeDevice();

        UINT size = static_cast<UINT>(layout.getElements().size());
        D3D11_INPUT_ELEMENT_DESC* ieds = new D3D11_INPUT_ELEMENT_DESC[size];
        unsigned int i = 0;
        for (const auto& element : layout)
        {
            ieds[i].SemanticName = element.Name.c_str();
            ieds[i].SemanticIndex = 0;
            ieds[i].Format = dataTypeToDXGIFormat(element.Type);
            ieds[i].AlignedByteOffset = static_cast<UINT>(element.Offset);
            ieds[i].InputSlot = 0;
            ieds[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            ieds[i].InstanceDataStepRate = 0;
            ++i;
        }

        const auto& d3dShader = reinterpret_cast<const Direct3DShader&>(shader);
        auto& blob = d3dShader.getVertexShaderByteCode();
        hr = device->CreateInputLayout(
            ieds,
            size,
            blob->GetBufferPointer(),
            blob->GetBufferSize(),
            &m_nativeLayout);
        JNG_D3D_CHECK_HR(hr);

        delete[] ieds;
    }

    // NOTE: this need to be here for com::wrl to work
    Direct3DVertexArray::~Direct3DVertexArray() = default;

    void Direct3DVertexArray::bind() const
    {
        const auto& deviceContext = m_graphicsContext->getNativeDeviceContext();

        deviceContext->IASetInputLayout(m_nativeLayout.Get());
        m_VBO->bind();
        m_IBO->bind();
        // TODO: implement
    }

    void Direct3DVertexArray::unbind() const
    {
        // TODO: implement
    }

    void Direct3DVertexArray::setIndexBuffer(const Ref<IndexBuffer>& ibo)
    {
        m_IBO = ibo;

        // TODO: implement
    }

} // namespace jng
