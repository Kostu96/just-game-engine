/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"

#include <string>
#include <vector>

namespace jng {

    class IndexBuffer;
    class VertexBuffer;
    class Shader;
    class VertexArrayImpl;

    struct LayoutElement
    {
        enum class DataType
        {
            Float, Float2, Float3, Float4,
            Mat3, Mat4,
            Int, Int2, Int3, Int4,
            Bool
        };

        LayoutElement(DataType type, const char* name, bool normalized = false);

        static size_t dataTypeToSize(DataType type);

        std::string Name;
        DataType Type;
        size_t Size;
        uintptr_t Offset;
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
        void bind() const;
        void unbind() const;

        const Ref<VertexBuffer>& getVertexBuffer() const;
        void setIndexBuffer(const Ref<IndexBuffer>& ibo);
        const Ref<IndexBuffer>& getIndexBuffer() const;

        static Ref<VertexArray> create(const Ref<VertexBuffer>& vbo, const VertexLayout& layout, const Ref<Shader>& shader);
        explicit VertexArray(Scope<VertexArrayImpl>&& implementation);
        ~VertexArray();
    private:
        Scope<VertexArrayImpl> m_implementation;
    };

} // namespace jng
