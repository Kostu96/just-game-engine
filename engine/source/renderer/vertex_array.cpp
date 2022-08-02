/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vertex_array.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/opengl/vertex_array_ogl.hpp"
//#include "renderer/vulkan/vertex_array_vlk.hpp"

#if defined(JNG_WINDOWS)
#include "renderer/direct3d/vertex_array_d3d.hpp"
#endif

namespace jng {

    LayoutElement::LayoutElement(DataType type, const char* name, bool passAsFloat, bool normalized) :
        Name{ name },
        Type{ type },
        Size{ dataTypeToSize(type) },
        Offset{ 0 },
        PassAsFloat{ passAsFloat },
        Normalized{ normalized } {}

    size_t LayoutElement::dataTypeToSize(DataType type)
    {
        switch (type)
        {
        case DataType::Float:   return sizeof(float);
        case DataType::Float2:  return sizeof(float) * 2;
        case DataType::Float3:  return sizeof(float) * 3;
        case DataType::Float4:  return sizeof(float) * 4;
        case DataType::UInt:
        case DataType::UInt4x8: return sizeof(uint32);
        case DataType::UInt2:   return sizeof(uint32) * 2;
        case DataType::UInt3:   return sizeof(uint32) * 3;
        case DataType::UInt4:   return sizeof(uint32) * 4;
        case DataType::Int:     return sizeof(int32);
        case DataType::Int2:    return sizeof(int32) * 2;
        case DataType::Int3:    return sizeof(int32) * 3;
        case DataType::Int4:    return sizeof(int32) * 4;
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

    VertexLayout::VertexLayout(const VertexLayout& other) :
        m_elements{ other.m_elements },
        m_stride{ other.m_stride } {}

    Ref<VertexArray> VertexArray::create(const Ref<VertexBuffer>& vbo, const VertexLayout& layout, const Ref<Shader>& shader)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DVertexArray>(vbo, layout, shader);
#endif
        case RendererBackend::OpenGL: return makeRef<OpenGLVertexArray>(vbo, layout, shader);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

} // namespace jng
