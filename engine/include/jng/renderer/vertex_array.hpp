/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

#include <string>
#include <vector>

namespace jng {

    class IndexBuffer;
    class VertexBuffer;
    class Shader;

    struct LayoutElement
    {
        enum class DataType
        {
            Float, Float2, Float3, Float4,
            Int,   Int2,   Int3,   Int4,
            UInt,  UInt2,  UInt3,  UInt4,
            UInt4x8
        };

        LayoutElement(DataType type, const char* name, bool passAsFloat = false, bool normalized = false);

        static size_t dataTypeToSize(DataType type);

        std::string Name;
        DataType Type;
        size_t Size;
        uintptr_t Offset;
        bool PassAsFloat;
        bool Normalized;
    };

    // TODO: Merge VartexArray and VertexLayout? What name would be the best?
    class VertexLayout
    {
    public:
        using ContainerType = std::vector<LayoutElement>;
        using ContainerConstInterator = ContainerType::const_iterator;

        VertexLayout() = default;
        VertexLayout(const std::initializer_list<LayoutElement>& list);
        VertexLayout(const VertexLayout& other);

        const ContainerType& getElements() const { return m_elements; }
        uint32 getStride() const { return m_stride; }

        ContainerConstInterator begin() const { return m_elements.begin(); }
        ContainerConstInterator end() const { return m_elements.end(); }
    private:
        ContainerType m_elements;
        uint32 m_stride{};
    };

    class VertexArray
    {
    public:
        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual const Ref<VertexBuffer>& getVertexBuffer() const = 0;
        virtual void setIndexBuffer(const Ref<IndexBuffer>& ibo) = 0;
        virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

        static Ref<VertexArray> create(const Ref<VertexBuffer>& vbo, const VertexLayout& layout, const Ref<Shader>& shader);
        virtual ~VertexArray() = default;
    };

} // namespace jng
