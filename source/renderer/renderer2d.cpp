/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/renderer2d.hpp"

#include "core/base_internal.hpp"
#include "renderer/buffers.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/vertex_array.hpp"

#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace jng {

    struct QuadVertex
    {
        glm::vec2 position;
        glm::vec2 texCoord;
        glm::vec4 color; // TODO: convert to unorm format
        float texIndex; // TODO: try to make this uint32 or even uint8 (glVertexAttribIPointer)
    };

    struct RenderData
    {
        static constexpr uint32 MaxQuadsPerBatch = 5000;
        //static constexpr uint32 TrisVertexCount = 3;
        static constexpr uint32 QuadVertexCount = 4;
        static constexpr uint32 QuadIndexCount = 6;
        static constexpr uint32 MaxVerticesPerBatch = QuadVertexCount * MaxQuadsPerBatch;
        static constexpr uint32 MaxIndicesPerBatch = QuadIndexCount * MaxQuadsPerBatch;
        static constexpr uint32 MaxTextureSlots = 16; // TODO: render caps

        Ref<VertexArray> quadVAO;
        Ref<VertexBuffer> quadVBO;
        Ref<UniformBuffer> quadUBO;
        Ref<Texture> whiteTexture;
        Ref<Shader> shader;

        uint32 currentQuadIndexCount = 0;
        QuadVertex* quadVBOBase = nullptr;
        QuadVertex* quadVBOPtr = nullptr;
        glm::vec4 quadVertexPositions[QuadVertexCount]{};

        std::array<Ref<Texture>, MaxTextureSlots> textureSlots;
        uint8 textureSlotIndex = 1; // 0 = white texture

        Renderer2D::Statistics statistics;
    };

    static RenderData s_data;

    void Renderer2D::init()
    {
        JNG_PROFILE_FUNCTION();

        s_data.shader = Shader::create("assets/shaders/basic_vertex.glsl", "assets/shaders/basic_fragment.glsl");
        s_data.quadUBO = UniformBuffer::create(sizeof(glm::mat4));
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
        s_data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        uint32* quadIndices = new uint32[RenderData::MaxIndicesPerBatch];
        for (uint32 i = 0, offset = 0; i < s_data.MaxIndicesPerBatch; i += s_data.QuadIndexCount, offset += s_data.QuadVertexCount)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 2;
            quadIndices[i + 2] = offset + 1;

            quadIndices[i + 3] = offset + 0;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 2;
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
        s_data.quadUBO->bind(0);
    }

    void Renderer2D::shutdown()
    {
        delete[] s_data.quadVBOBase;
    }

    void Renderer2D::beginScene(const Camera& camera)
    {
        JNG_PROFILE_FUNCTION();

        s_data.quadUBO->setData(glm::value_ptr(camera.getVP()), sizeof(glm::mat4));

        s_data.statistics.drawCalls = 0;
        s_data.statistics.quadCount = 0;
        beginBatch();
    }

    void Renderer2D::endScene()
    {
        JNG_PROFILE_FUNCTION();

        endBatch();
    }

    void Renderer2D::fillQuad(const Properties& properties)
    {
        JNG_PROFILE_FUNCTION();

        if (s_data.currentQuadIndexCount >= RenderData::MaxIndicesPerBatch)
        {
            endBatch();
            beginBatch();
        }

        uint32 textureIndex = static_cast<uint32>(-1);
        for (uint32 i = 0; i < s_data.textureSlotIndex; ++i)
            if (s_data.textureSlots[i]->getID() == properties.texture->getID())
            {
                textureIndex = i;
                break;
            }

        if (textureIndex == static_cast<uint32>(-1))
        {
            s_data.textureSlots[s_data.textureSlotIndex] = properties.texture;
            textureIndex = s_data.textureSlotIndex++;
        }

        for (uint32 i = 0; i < RenderData::QuadVertexCount; ++i)
        {
            s_data.quadVBOPtr->position = properties.quadVertexPositions[i];
            s_data.quadVBOPtr->texCoord = properties.textureCoords[i];
            s_data.quadVBOPtr->color = properties.color;
            s_data.quadVBOPtr->texIndex = static_cast<float>(textureIndex);
            ++s_data.quadVBOPtr;
        }

        s_data.currentQuadIndexCount += RenderData::QuadIndexCount;

        ++s_data.statistics.quadCount;
    }

    void Renderer2D::fillQuad(glm::vec2 position, glm::vec2 size, const glm::vec4& color)
    {
        fillQuad(position, size, s_data.whiteTexture, color);
    }

    void Renderer2D::fillQuad(glm::vec2 position, glm::vec2 size, float rotation, const glm::vec4& color)
    {
        fillQuad(position, size, rotation, s_data.whiteTexture, color);
    }

    void Renderer2D::fillQuad(glm::vec2 position, glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color)
    {
        glm::vec2 quadVertexPositions[RenderData::QuadVertexCount];

        quadVertexPositions[0] = { position.x, position.y };
        quadVertexPositions[1] = { position.x + size.x, position.y };
        quadVertexPositions[2] = { position.x + size.x, position.y + size.y };
        quadVertexPositions[3] = { position.x, position.y + size.y };

        constexpr glm::vec2 texCoords[RenderData::QuadVertexCount] = {
            { 0.f, 0.f },
            { 1.f, 0.f },
            { 1.f, 1.f },
            { 0.f, 1.f }
        };

        const Properties properties{
            quadVertexPositions,
            texCoords,
            texture,
            color
        };
        fillQuad(properties);
    }

    void Renderer2D::fillQuad(glm::vec2 position, glm::vec2 size, float rotation, const Ref<Texture>& texture, const glm::vec4& color)
    {
        glm::vec2 quadVertexPositions[RenderData::QuadVertexCount];

        glm::mat4 model =
            glm::rotate(
                glm::scale(
                    glm::translate(glm::mat4(1.f), glm::vec3(position, 0.f)),
                    glm::vec3(size, 1.0f)),
                glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)
            );

        for (uint32 i = 0; i < RenderData::QuadVertexCount; ++i)
            quadVertexPositions[i] = model * s_data.quadVertexPositions[i];

        constexpr glm::vec2 texCoords[RenderData::QuadVertexCount] = {
            { 0.f, 0.f },
            { 1.f, 0.f },
            { 1.f, 1.f },
            { 0.f, 1.f }
        };

        const Properties properties{
            quadVertexPositions,
            texCoords,
            texture,
            color
        };
        fillQuad(properties);
    }

    const Renderer2D::Statistics& Renderer2D::getStatistics()
    {
        return s_data.statistics;
    }

    void Renderer2D::beginBatch()
    {
        s_data.currentQuadIndexCount = 0;
        s_data.quadVBOPtr = s_data.quadVBOBase;
        s_data.textureSlotIndex = 1;
    }

    void Renderer2D::endBatch()
    {
        for (uint32 i = 1; i < s_data.textureSlotIndex; ++i)
            s_data.textureSlots[i]->bind(i);

        size_t dataSize = static_cast<size_t>(
            reinterpret_cast<uint8*>(s_data.quadVBOPtr) -
            reinterpret_cast<uint8*>(s_data.quadVBOBase)
        );
        s_data.quadVBO->setData(s_data.quadVBOBase, dataSize);

        s_data.quadVAO->bind();
        RendererAPI::drawIndexed(s_data.currentQuadIndexCount);

        ++s_data.statistics.drawCalls;
    }

} // namespace jng
