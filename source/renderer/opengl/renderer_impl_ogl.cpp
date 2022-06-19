/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/opengl/renderer_impl_ogl.hpp"

#include "renderer/opengl/buffers_ogl.hpp"

#include <array>
#include <glm/gtc/matrix_transform.hpp>

namespace jng {

    struct QuadVertex
    {
        glm::vec2 position;
        glm::vec2 texCoord;
        glm::vec4 color; // convert to unorm format
        float texIndex; // TODO: try to make this uint32 or even uint8 (glVertexAttribIPointer)
    };

    struct RenderData
    {
        static constexpr uint32 MaxQuadsPerBatch = 5000;
        static constexpr uint32 QuadVertexCount = 4;
        static constexpr uint32 QuadIndexCount = 6;
        static constexpr uint32 MaxVerticesPerBatch = QuadVertexCount * MaxQuadsPerBatch;
        static constexpr uint32 MaxIndicesPerBatch = QuadIndexCount * MaxQuadsPerBatch;
        static constexpr uint32 MaxTextureSlots = 32; // TODO: render caps

        Ref<VertexArray> quadVAO;
        Ref<VertexBuffer> quadVBO;
        Ref<Texture> whiteTexture;
        Ref<Shader> shader;

        uint32 currentQuadIndexCount = 0;
        QuadVertex* quadVBOBase = nullptr;
        QuadVertex* quadVBOPtr = nullptr;
        glm::vec4 quadVertexPositions[QuadVertexCount]{};

        std::array<Ref<Texture>, MaxTextureSlots> textureSlots;
        uint8 textureSlotIndex = 1; // 0 = white texture

        glm::mat4 VP{}; // TODO: don't need it?

        Renderer::Statistics statistics;
    };

    static RenderData s_data;

    OpenGLRendererImpl::OpenGLRendererImpl()
    {
        s_data.shader = Shader::create(k2d::shader_source::basic_vert, shader_source::basic_frag);
        s_data.quadVBO = VertexBuffer::create(RenderData::MaxVerticesPerBatch * sizeof(QuadVertex));

        VertexLayout vertexLayout = {
            { LayoutElement::DataType::Float2, "a_Position" },
            { LayoutElement::DataType::Float2, "a_TexCoord" },
            { LayoutElement::DataType::Float4, "a_Color" },
            { LayoutElement::DataType::Float, "a_TexIndex" }
        };
        s_data.quadVAO = VertexArray::create(s_data.quadVBO, vertexLayout, s_data.shader);

        s_data.quadVBOBase = new QuadVertex[s_data.MaxVerticesPerBatch];
        s_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_data.quadVertexPositions[1] = { -0.5f,  0.5f, 0.0f, 1.0f };
        s_data.quadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        s_data.quadVertexPositions[3] = { 0.5f, -0.5f, 0.0f, 1.0f };

        uint32* quadIndices = new uint32[RenderData::MaxIndicesPerBatch];
        for (uint32 i = 0, offset = 0; i < s_data.MaxIndicesPerBatch; i += s_data.QuadIndexCount, offset += s_data.QuadVertexCount)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 0;
            quadIndices[i + 4] = offset + 2;
            quadIndices[i + 5] = offset + 3;
        }
        auto quadIBO = IndexBuffer::create(quadIndices, RenderData::MaxIndicesPerBatch);
        s_data.quadVAO->setIndexBuffer(quadIBO);
        delete[] quadIndices;

        s_data.whiteTexture = Texture::create(1, 1);
        uint32 whiteTextureData = 0xffffffff;
        s_data.whiteTexture->setData(&whiteTextureData, sizeof(uint32));
        s_data.textureSlots[0] = s_data.whiteTexture;
        s_data.whiteTexture->bind(0);

        s_data.shader->bind();
        int32 samplers[s_data.MaxTextureSlots];
        for (uint32 i = 0; i < s_data.MaxTextureSlots; ++i)
            samplers[i] = static_cast<int32>(i);
        s_data.shader->set("u_Textures", samplers, RenderData::MaxTextureSlots);
    }

} // namespace jng
