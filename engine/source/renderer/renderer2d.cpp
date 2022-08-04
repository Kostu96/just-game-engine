/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/renderer2d.hpp"

#include "core/base_internal.hpp"
#include "core/engine.hpp"
#include "renderer/buffers.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/vertex_array.hpp"
#include "scene/components.hpp"

#include <array>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/packing.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace jng {

    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec2 texCoord;
        uint32 color;
        uint32 texIndex;
        int32 entityID;
    };

    struct CircleVertex
    {
        glm::vec3 position;
        glm::vec2 localPosition;
        float thickness;
        float fade;
        uint32 color;
        int32 entityID;
    };

    struct LineVertex
    {
        glm::vec3 position;
        uint32 color;
    };

    struct RenderData
    {
        static constexpr uint32 MaxQuadsPerBatch =   1000; //
        static constexpr uint32 MaxCirclesPerBatch = 500;  //
        static constexpr uint32 MaxLinesPerBatch =   2000; // NOTE: arbitrarily choosen values
        static constexpr uint32 QuadAndCircleVertexCount = 4;
        static constexpr uint32 LineVertexCount = 2;
        static constexpr uint32 QuadAndCircleIndexCount = 6;
        static constexpr uint32 MaxQuadVerticesPerBatch = QuadAndCircleVertexCount * MaxQuadsPerBatch;
        static constexpr uint32 MaxQuadIndicesPerBatch = QuadAndCircleIndexCount * MaxQuadsPerBatch;
        static constexpr uint32 MaxCircleVerticesPerBatch = QuadAndCircleVertexCount * MaxCirclesPerBatch;
        static constexpr uint32 MaxCircleIndicesPerBatch = QuadAndCircleIndexCount * MaxCirclesPerBatch;
        static constexpr uint32 MaxLineVerticesPerBatch = LineVertexCount * MaxLinesPerBatch;
        static constexpr uint32 MaxTextureSlots = 16; // TODO: render caps

        Ref<UniformBuffer> cameraUBO;
        Ref<Texture> whiteTexture;

        glm::vec4 quadAndCircleVertexPositions[4]{};

        Ref<Shader> quadShader;
        Ref<VertexBuffer> quadVBO;
        Ref<VertexArray> quadVAO;
        uint32 currentQuadIndexCount = 0;
        QuadVertex* quadVBOBase = nullptr;
        QuadVertex* quadVBOPtr = nullptr;

        Ref<Shader> circleShader;
        Ref<VertexBuffer> circleVBO;
        Ref<VertexArray> circleVAO;
        uint32 currentCircleIndexCount = 0;
        CircleVertex* circleVBOBase = nullptr;
        CircleVertex* circleVBOPtr = nullptr;
        
        Ref<Shader> lineShader;
        Ref<VertexBuffer> lineVBO;
        Ref<VertexArray> lineVAO;
        uint32 currentLineVertexCount = 0;
        LineVertex* lineVBOBase = nullptr;
        LineVertex* lineVBOPtr = nullptr;

        std::array<Ref<Texture>, MaxTextureSlots> textureSlots;
        uint8 textureSlotIndex = 1; // 0 = white texture

        Renderer2D::Statistics statistics;
    };

    static RenderData s_data;

    void Renderer2D::init()
    {
        JNG_PROFILE_FUNCTION();

        std::filesystem::path assetsDir = Engine::get().getProperties().assetsDirectory;

        s_data.cameraUBO = UniformBuffer::create(sizeof(glm::mat4));

        Texture::Properties props{
            TextureFormat::RGBA8,
            1, 1
        };
        s_data.whiteTexture = Texture::create(props);
        uint32 whiteTextureData = 0xffffffff;
        s_data.whiteTexture->setData(&whiteTextureData, sizeof(uint32));
        s_data.textureSlots[0] = s_data.whiteTexture;

        s_data.quadAndCircleVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_data.quadAndCircleVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_data.quadAndCircleVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_data.quadAndCircleVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        // Quad
        s_data.quadShader = Shader::create(
            (assetsDir / "shaders/quad_vertex.glsl").string(),
            (assetsDir / "shaders/quad_fragment.glsl").string()
        );
        s_data.quadVBO = VertexBuffer::create(RenderData::MaxQuadVerticesPerBatch * sizeof(QuadVertex));

        VertexLayout quadVertexLayout = {
            { LayoutElement::DataType::Float3,  "a_Position" },
            { LayoutElement::DataType::Float2,  "a_TexCoord" },
            { LayoutElement::DataType::UInt4x8, "a_Color", true, true },
            { LayoutElement::DataType::UInt,    "a_TexIndex", false },
            { LayoutElement::DataType::Int,     "a_EntityID", false }
        };
        s_data.quadVAO = VertexArray::create(s_data.quadVBO, quadVertexLayout, s_data.quadShader);
        s_data.quadVBOBase = new QuadVertex[s_data.MaxQuadVerticesPerBatch];
        
        uint32* quadIndices = new uint32[RenderData::MaxQuadIndicesPerBatch];
        for (uint32 i = 0, offset = 0; i < s_data.MaxQuadIndicesPerBatch; i += s_data.QuadAndCircleIndexCount, offset += s_data.QuadAndCircleVertexCount)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 2;
            quadIndices[i + 2] = offset + 1;

            quadIndices[i + 3] = offset + 0;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 2;
        }
        auto quadIBO = IndexBuffer::create(quadIndices, RenderData::MaxQuadIndicesPerBatch);
        s_data.quadVAO->setIndexBuffer(quadIBO);
        delete[] quadIndices;

        // Circle
        s_data.circleShader = Shader::create(
            (assetsDir / "shaders/circle_vertex.glsl").string(),
            (assetsDir / "shaders/circle_fragment.glsl").string()
        );
        s_data.circleVBO = VertexBuffer::create(RenderData::MaxCircleVerticesPerBatch * sizeof(CircleVertex));

        VertexLayout circleVertexLayout = {
            { LayoutElement::DataType::Float3,  "a_Position" },
            { LayoutElement::DataType::Float2,  "a_LocalPosition" },
            { LayoutElement::DataType::Float,   "a_Thickness" },
            { LayoutElement::DataType::Float,   "a_Fade" },
            { LayoutElement::DataType::UInt4x8, "a_Color", true, true },
            { LayoutElement::DataType::Int,     "a_EntityID", false }
        };
        s_data.circleVAO = VertexArray::create(s_data.circleVBO, circleVertexLayout, s_data.circleShader);
        s_data.circleVBOBase = new CircleVertex[s_data.MaxCircleVerticesPerBatch];

        uint32* circleIndices = new uint32[RenderData::MaxCircleIndicesPerBatch];
        for (uint32 i = 0, offset = 0; i < s_data.MaxCircleIndicesPerBatch; i += s_data.QuadAndCircleIndexCount, offset += s_data.QuadAndCircleVertexCount)
        {
            circleIndices[i + 0] = offset + 0;
            circleIndices[i + 1] = offset + 2;
            circleIndices[i + 2] = offset + 1;

            circleIndices[i + 3] = offset + 0;
            circleIndices[i + 4] = offset + 3;
            circleIndices[i + 5] = offset + 2;
        }
        auto circleIBO = IndexBuffer::create(circleIndices, RenderData::MaxCircleIndicesPerBatch);
        s_data.circleVAO->setIndexBuffer(circleIBO); // TODO: Use the same index buffer as for quads?
        delete[] circleIndices;

        // Line
        s_data.lineShader = Shader::create(
            (assetsDir / "shaders/line_vertex.glsl").string(),
            (assetsDir / "shaders/line_fragment.glsl").string()
        );
        s_data.lineVBO = VertexBuffer::create(RenderData::MaxLineVerticesPerBatch * sizeof(LineVertex));

        VertexLayout lineVertexLayout = {
            { LayoutElement::DataType::Float3,  "a_Position" },
            { LayoutElement::DataType::UInt4x8, "a_Color", true, true }
        };
        s_data.lineVAO = VertexArray::create(s_data.lineVBO, lineVertexLayout, s_data.lineShader);
        s_data.lineVBOBase = new LineVertex[s_data.MaxLineVerticesPerBatch];

        s_data.cameraUBO->bind(0);
    }

    void Renderer2D::shutdown()
    {
        delete[] s_data.quadVBOBase;
        delete[] s_data.circleVBOBase;
        delete[] s_data.lineVBOBase;
    }

    void Renderer2D::beginScene(const glm::mat4& viewProjection)
    {
        JNG_PROFILE_FUNCTION();

        s_data.cameraUBO->setData(glm::value_ptr(viewProjection), sizeof(glm::mat4));

        s_data.statistics.drawCalls = 0;
        s_data.statistics.quadCount = 0;
        s_data.statistics.circleCount = 0;
        s_data.statistics.lineCount = 0;
        beginQuadBatch();
        beginCircleBatch();
        beginLineBatch();
    }

    void Renderer2D::endScene()
    {
        JNG_PROFILE_FUNCTION();

        endQuadBatch();
        endCircleBatch();
        endLineBatch();
    }

    void Renderer2D::drawSprite(const glm::mat4& transform, const SpriteRendererComponent& src, int32 entityID)
    {
        glm::vec3 quadVertexPositions[RenderData::QuadAndCircleVertexCount];

        for (uint32 i = 0; i < RenderData::QuadAndCircleVertexCount; ++i)
            quadVertexPositions[i] = transform * s_data.quadAndCircleVertexPositions[i];

        constexpr glm::vec2 texCoords[RenderData::QuadAndCircleVertexCount] = {
            { 0.f, 0.f },
            { 1.f, 0.f },
            { 1.f, 1.f },
            { 0.f, 1.f }
        };

        const DrawQuadProperties properties{
            quadVertexPositions,
            texCoords,
            src.texture ? src.texture : s_data.whiteTexture,
            glm::packUnorm4x8(src.Color),
            entityID
        };
        drawQuad(properties);
    }

    void Renderer2D::drawCircle(const glm::mat4& transform, const CircleRendererComponent& crc, int32 entityID)
    {
        glm::vec3 circleVertexPositions[RenderData::QuadAndCircleVertexCount];
        glm::vec2 circleVertexLocalPositions[RenderData::QuadAndCircleVertexCount];

        for (uint32 i = 0; i < RenderData::QuadAndCircleVertexCount; ++i)
        {
            circleVertexPositions[i] = transform * s_data.quadAndCircleVertexPositions[i];
            circleVertexLocalPositions[i] = s_data.quadAndCircleVertexPositions[i] * 2.f;
        }

        const DrawCircleProperties properties{
            circleVertexPositions,
            circleVertexLocalPositions,
            crc.thickness,
            crc.fade,
            glm::packUnorm4x8(crc.color),
            entityID
        };
        drawCircle(properties);
    }

    void Renderer2D::drawQuad(const DrawQuadProperties& properties)
    {
        JNG_PROFILE_FUNCTION();

        if (s_data.currentQuadIndexCount >= RenderData::MaxQuadIndicesPerBatch)
        {
            endQuadBatch();
            beginQuadBatch();
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

        for (uint32 i = 0; i < RenderData::QuadAndCircleVertexCount; ++i)
        {
            s_data.quadVBOPtr->position = properties.vertexPositions[i];
            s_data.quadVBOPtr->texCoord = properties.textureCoords[i];
            s_data.quadVBOPtr->color = properties.color;
            s_data.quadVBOPtr->texIndex = textureIndex;
            s_data.quadVBOPtr->entityID = properties.entityID;
            ++s_data.quadVBOPtr;
        }

        s_data.currentQuadIndexCount += RenderData::QuadAndCircleIndexCount;

        ++s_data.statistics.quadCount;
    }

    void Renderer2D::drawCircle(const DrawCircleProperties& properties)
    {
        JNG_PROFILE_FUNCTION();

        if (s_data.currentCircleIndexCount >= RenderData::MaxCircleIndicesPerBatch)
        {
            endCircleBatch();
            beginCircleBatch();
        }

        for (uint32 i = 0; i < RenderData::QuadAndCircleVertexCount; ++i)
        {
            s_data.circleVBOPtr->position = properties.vertexPositions[i];
            s_data.circleVBOPtr->localPosition = properties.localVertexPositions[i];
            s_data.circleVBOPtr->thickness = properties.thickness;
            s_data.circleVBOPtr->fade = properties.fade;
            s_data.circleVBOPtr->color = properties.color;
            s_data.circleVBOPtr->entityID = properties.entityID;
            ++s_data.circleVBOPtr;
        }

        s_data.currentCircleIndexCount += RenderData::QuadAndCircleIndexCount;

        ++s_data.statistics.circleCount;
    }

    void Renderer2D::drawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
    {
        JNG_PROFILE_FUNCTION();

        if (s_data.currentLineVertexCount >= RenderData::MaxLineVerticesPerBatch)
        {
            endLineBatch();
            beginLineBatch();
        }

        s_data.lineVBOPtr->position = p0;
        s_data.lineVBOPtr->color = glm::packUnorm4x8(color);
        s_data.lineVBOPtr++;
        s_data.lineVBOPtr->position = p1;
        s_data.lineVBOPtr->color = glm::packUnorm4x8(color);
        s_data.lineVBOPtr++;

        s_data.currentLineVertexCount += RenderData::LineVertexCount;

        ++s_data.statistics.lineCount;
    }

    void Renderer2D::drawRect(const glm::mat4& transform, const glm::vec4& color)
    {
        glm::vec3 p0 = transform * s_data.quadAndCircleVertexPositions[0];
        glm::vec3 p1 = transform * s_data.quadAndCircleVertexPositions[1];
        glm::vec3 p2 = transform * s_data.quadAndCircleVertexPositions[2];
        glm::vec3 p3 = transform * s_data.quadAndCircleVertexPositions[3];

        drawLine(p0, p1, color);
        drawLine(p1, p2, color);
        drawLine(p2, p3, color);
        drawLine(p3, p0, color);
    }

    void Renderer2D::fillQuad(glm::vec3 position, glm::vec2 size, const glm::vec4& color)
    {
        fillQuad(position, size, s_data.whiteTexture, color);
    }

    void Renderer2D::fillQuad(glm::vec3 position, glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color)
    {
        glm::vec3 quadVertexPositions[RenderData::QuadAndCircleVertexCount];

        quadVertexPositions[0] = { position.x,          position.y,          position.z };
        quadVertexPositions[1] = { position.x + size.x, position.y,          position.z };
        quadVertexPositions[2] = { position.x + size.x, position.y + size.y, position.z };
        quadVertexPositions[3] = { position.x,          position.y + size.y, position.z };

        constexpr glm::vec2 texCoords[RenderData::QuadAndCircleVertexCount] = {
            { 0.f, 0.f },
            { 1.f, 0.f },
            { 1.f, 1.f },
            { 0.f, 1.f }
        };

        const DrawQuadProperties properties{
            quadVertexPositions,
            texCoords,
            texture,
            glm::packUnorm4x8(color)
        };
        drawQuad(properties);
    }

    void Renderer2D::fillQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        fillQuad(transform, s_data.whiteTexture, color);
    }

    void Renderer2D::fillQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& color)
    {
        glm::vec3 quadVertexPositions[RenderData::QuadAndCircleVertexCount];

        for (uint32 i = 0; i < RenderData::QuadAndCircleVertexCount; ++i)
            quadVertexPositions[i] = transform * s_data.quadAndCircleVertexPositions[i];

        constexpr glm::vec2 texCoords[RenderData::QuadAndCircleVertexCount] = {
            { 0.f, 0.f },
            { 1.f, 0.f },
            { 1.f, 1.f },
            { 0.f, 1.f }
        };

        const DrawQuadProperties properties{
            quadVertexPositions,
            texCoords,
            texture,
            glm::packUnorm4x8(color)
        };
        drawQuad(properties);
    }

    const Renderer2D::Statistics& Renderer2D::getStatistics()
    {
        return s_data.statistics;
    }

    void Renderer2D::beginQuadBatch()
    {
        s_data.currentQuadIndexCount = 0;
        s_data.quadVBOPtr = s_data.quadVBOBase;
        s_data.textureSlotIndex = 1;
    }

    void Renderer2D::endQuadBatch()
    {
        for (uint32 i = 0; i < s_data.textureSlotIndex; ++i)
            s_data.textureSlots[i]->bind(i);

        // TODO: check if ptrdiff_t will be the same here
        size_t dataSize = static_cast<size_t>(
            reinterpret_cast<uint8*>(s_data.quadVBOPtr) -
            reinterpret_cast<uint8*>(s_data.quadVBOBase)
        );
        s_data.quadVBO->setData(s_data.quadVBOBase, dataSize);

        s_data.quadVAO->bind();
        s_data.quadShader->bind();
        RendererAPI::drawIndexed(s_data.currentQuadIndexCount);

        ++s_data.statistics.drawCalls;
    }

    void Renderer2D::beginCircleBatch()
    {
        s_data.currentCircleIndexCount = 0;
        s_data.circleVBOPtr = s_data.circleVBOBase;
    }

    void Renderer2D::endCircleBatch()
    {
        // TODO: check if ptrdiff_t will be the same here
        size_t dataSize = static_cast<size_t>(
            reinterpret_cast<uint8*>(s_data.circleVBOPtr) -
            reinterpret_cast<uint8*>(s_data.circleVBOBase)
            );
        s_data.circleVBO->setData(s_data.circleVBOBase, dataSize);

        s_data.circleVAO->bind();
        s_data.circleShader->bind();
        RendererAPI::drawIndexed(s_data.currentCircleIndexCount);

        ++s_data.statistics.drawCalls;
    }

    void Renderer2D::beginLineBatch()
    {
        s_data.currentLineVertexCount = 0;
        s_data.lineVBOPtr = s_data.lineVBOBase;
    }

    void Renderer2D::endLineBatch()
    {
        // TODO: check if ptrdiff_t will be the same here
        size_t dataSize = static_cast<size_t>(
            reinterpret_cast<uint8*>(s_data.lineVBOPtr) -
            reinterpret_cast<uint8*>(s_data.lineVBOBase)
            );
        s_data.lineVBO->setData(s_data.lineVBOBase, dataSize);

        s_data.lineVAO->bind();
        s_data.lineShader->bind();
        RendererAPI::draw(s_data.currentLineVertexCount, RendererAPI::PrimitiveType::Lines);

        ++s_data.statistics.drawCalls;
    }

} // namespace jng
