/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vertex_array.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"
//#include "renderer/direct3d/vertex_array_impl_d3d.hpp"
#include "renderer/opengl/vertex_array_ogl.hpp"
//#include "renderer/vulkan/vertex_array_impl_vlk.hpp"

namespace jng {

    LayoutElement::LayoutElement(DataType type, const char* name, bool normalized) :
        Name(name),
        Type(type),
        Size(dataTypeToSize(type)),
        Offset(0),
        Normalized(normalized) {}

    size_t LayoutElement::dataTypeToSize(DataType type)
    {
        switch (type)
        {
        case DataType::Float:  return sizeof(float);
        case DataType::Float2: return sizeof(float) * 2;
        case DataType::Float3: return sizeof(float) * 3;
        case DataType::Float4: return sizeof(float) * 4;
        case DataType::Mat3:   return sizeof(float) * 3 * 3;
        case DataType::Mat4:   return sizeof(float) * 4 * 4;
        case DataType::Int:    return sizeof(int);
        case DataType::Int2:   return sizeof(int) * 2;
        case DataType::Int3:   return sizeof(int) * 3;
        case DataType::Int4:   return sizeof(int) * 4;
        case DataType::Bool:   return sizeof(bool);
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!")
            return 0;
    }

    VertexLayout::VertexLayout(const std::initializer_list<LayoutElement>& list) :
        m_elements(list)
    {
        unsigned int offset = 0;
        m_stride = 0;
        for (auto& element : m_elements)
        {
            element.Offset = offset;
            offset += static_cast<unsigned int>(element.Size);
            m_stride += static_cast<unsigned int>(element.Size);
        }
    }

    Ref<VertexArray> VertexArray::create(const Ref<VertexBuffer>& vbo, const VertexLayout& layout, const Ref<Shader>& shader)
    {
        JNG_PROFILE_FUNCTION();

        return makeRef<OpenGLVertexArray>(vbo, layout, shader);
    }

} // namespace jng
