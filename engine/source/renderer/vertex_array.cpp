/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vertex_array.hpp"

#include "core/base_internal.hpp"

namespace jng {

    LayoutElement::LayoutElement(DataType inType, const char* inName, bool inPassAsFloat, bool inNormalized) :
        name{ inName },
        type{ inType },
        size{ dataTypeToSize(type) },
        offset{ 0 },
        passAsFloat{ inPassAsFloat },
        normalized{ inNormalized } {}

    size_t LayoutElement::dataTypeToSize(DataType type)
    {
        switch (type)
        {
        case DataType::Float:   return sizeof(f32);
        case DataType::Float2:  return sizeof(f32) * 2;
        case DataType::Float3:  return sizeof(f32) * 3;
        case DataType::Float4:  return sizeof(f32) * 4;
        case DataType::UInt:
        case DataType::UInt4x8: return sizeof(u32);
        case DataType::UInt2:   return sizeof(u32) * 2;
        case DataType::UInt3:   return sizeof(u32) * 3;
        case DataType::UInt4:   return sizeof(u32) * 4;
        case DataType::Int:     return sizeof(s32);
        case DataType::Int2:    return sizeof(s32) * 2;
        case DataType::Int3:    return sizeof(s32) * 3;
        case DataType::Int4:    return sizeof(s32) * 4;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return 0;
    }

    VertexLayout::VertexLayout(const std::initializer_list<LayoutElement>& list) :
        m_elements(list)
    {
        unsigned int offset = 0;
        m_stride = 0;
        for (auto& element : m_elements)
        {
            element.offset = offset;
            offset += static_cast<unsigned int>(element.size);
            m_stride += static_cast<unsigned int>(element.size);
        }
    }

    VertexLayout::VertexLayout(const VertexLayout& other) :
        m_elements{ other.m_elements },
        m_stride{ other.m_stride } {}

} // namespace jng
