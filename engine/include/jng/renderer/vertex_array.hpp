/*
 * Copyright (C) 2021-2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

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

        LayoutElement(DataType inType, bool inPassAsFloat = false, bool inNormalized = false);

        static size_t dataTypeToSize(DataType type); // TODO: move to util functions?

        DataType type;
        size_t size;
        uintptr_t offset;
        bool passAsFloat;
        bool normalized;
    };

    class VertexLayout final
    {
    public:
        using ContainerType = std::vector<LayoutElement>;
        using ContainerConstInterator = ContainerType::const_iterator;

        VertexLayout() = default;
        VertexLayout(const std::initializer_list<LayoutElement>& list);
        VertexLayout(const VertexLayout& other);

        const ContainerType& getElements() const { return m_elements; }
        u32 getStride() const { return m_stride; }

        ContainerConstInterator begin() const { return m_elements.begin(); }
        ContainerConstInterator end() const { return m_elements.end(); }
    private:
        ContainerType m_elements;
        u32 m_stride;
    };

    class VertexArray final
    {
    public:
        VertexArray(const Ref<VertexBuffer>& vbo, const VertexLayout& layout);
        ~VertexArray();

        void bind() const;
        void unbind() const;

        const Ref<VertexBuffer>& getVertexBuffer() const { return m_VBO; }
        void setIndexBuffer(const Ref<IndexBuffer>& ibo);
        const Ref<IndexBuffer>& getIndexBuffer() const { return m_IBO; }
    private:
        u32 m_id;
        Ref<VertexBuffer> m_VBO;
        Ref<IndexBuffer> m_IBO;
    };

} // namespace jng
