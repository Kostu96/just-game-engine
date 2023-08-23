/*
 * Copyright (C) 2021-2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/renderer2d.hpp"

#include "core/base_internal.hpp"
#include "core/engine.hpp"
#include "renderer/buffers.hpp"
#include "renderer/font.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/vertex_array.hpp"
#include "scene/components.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/packing.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <msdf-atlas-gen/FontGeometry.h>

#include "renderer/font_data.inl"

namespace jng::Renderer2D {

    struct TextVertex
    {
        glm::vec3 position;
        glm::vec2 texCoord;
        u32 color;

        s32 entityID;
    };
    static_assert(sizeof(TextVertex) == 28);

    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec2 texCoord;
        u32 color;
        u32 texIndex;

        s32 entityID;
    };
    static_assert(sizeof(QuadVertex) == 32);

    struct CircleVertex
    {
        glm::vec3 position;
        glm::vec2 localPosition;
        f32 thickness;
        f32 fade;
        u32 color;

        s32 entityID;
    };
    static_assert(sizeof(CircleVertex) == 36);

    struct LineVertex
    {
        glm::vec3 position;
        u32 color;
    };
    static_assert(sizeof(LineVertex) == 16);

    struct RenderData
    {
        static constexpr u16 MaxQuadsPerBatch = 1000; //
        static constexpr u16 MaxLinesPerBatch = 2000; // NOTE: arbitrarily choosen values
        static constexpr u16 QuadVertexCount = 4;
        static constexpr u16 LineVertexCount = 2;
        static constexpr u16 QuadIndexCount  = 6;
        static constexpr u16 MaxQuadVerticesPerBatch = QuadVertexCount * MaxQuadsPerBatch;
        static constexpr u16 MaxQuadIndicesPerBatch  = QuadIndexCount  * MaxQuadsPerBatch;
        static constexpr u16 MaxLineVerticesPerBatch = LineVertexCount * MaxLinesPerBatch;
        static constexpr u16 MaxTextureSlots = 16; // TODO: render caps

        Ref<UniformBuffer> cameraUBO;
        Ref<Texture> whiteTexture;

        glm::vec4 quadVertexPositions[4]{};

        Ref<Shader> textShader;
        Ref<VertexBuffer> textVBO;
        Ref<VertexArray> textVAO;
        u16 currentTextIndexCount = 0;
        TextVertex* textVBOBase = nullptr;
        TextVertex* textVBOPtr = nullptr;

        Ref<Shader> quadShader;
        Ref<VertexBuffer> quadVBO;
        Ref<VertexArray> quadVAO;
        u16 currentQuadIndexCount = 0;
        QuadVertex* quadVBOBase = nullptr;
        QuadVertex* quadVBOPtr = nullptr;

        Ref<Shader> circleShader;
        Ref<VertexBuffer> circleVBO;
        Ref<VertexArray> circleVAO;
        u16 currentCircleIndexCount = 0;
        CircleVertex* circleVBOBase = nullptr;
        CircleVertex* circleVBOPtr = nullptr;
        
        Ref<Shader> lineShader;
        Ref<VertexBuffer> lineVBO;
        Ref<VertexArray> lineVAO;
        u16 currentLineVertexCount = 0;
        LineVertex* lineVBOBase = nullptr;
        LineVertex* lineVBOPtr = nullptr;

        std::array<Ref<Texture>, MaxTextureSlots> textureSlots;
        u8 textureSlotIndex = 1; // 0 = white texture

        Ref<Texture> fontAtlasTexture;

        Statistics statistics;
    };

    static RenderData s_data;

    static void beginTextBatch()
    {
        s_data.currentTextIndexCount = 0;
        s_data.textVBOPtr = s_data.textVBOBase;
    }

    static void endTextBatch()
    {
        size_t dataSize = (uintptr_t)s_data.textVBOPtr - (uintptr_t)s_data.textVBOBase;
        if (dataSize > 0)
        {
            s_data.textVBO->setData(s_data.textVBOBase, dataSize);

            s_data.fontAtlasTexture->bind(0);

            s_data.textVAO->bind();
            s_data.textShader->bind();
            RendererAPI::drawIndexed(s_data.currentTextIndexCount, s_data.textVAO->getIndexBuffer()->getIndexType());

            ++s_data.statistics.drawCalls;
        }
    }

    static void beginQuadBatch()
    {
        s_data.currentQuadIndexCount = 0;
        s_data.quadVBOPtr = s_data.quadVBOBase;
        s_data.textureSlotIndex = 1;
    }

    static void endQuadBatch()
    {
        size_t dataSize = (uintptr_t)s_data.quadVBOPtr - (uintptr_t)s_data.quadVBOBase;
        if (dataSize > 0)
        {
            s_data.quadVBO->setData(s_data.quadVBOBase, dataSize);

            for (u8 i = 0; i < s_data.textureSlotIndex; ++i)
                s_data.textureSlots[i]->bind(i);

            s_data.quadVAO->bind();
            s_data.quadShader->bind();
            RendererAPI::drawIndexed(s_data.currentQuadIndexCount, s_data.quadVAO->getIndexBuffer()->getIndexType());

            ++s_data.statistics.drawCalls;
        }
    }

    static void beginCircleBatch()
    {
        s_data.currentCircleIndexCount = 0;
        s_data.circleVBOPtr = s_data.circleVBOBase;
    }

    static void endCircleBatch()
    {
        size_t dataSize = (uintptr_t)s_data.circleVBOPtr - (uintptr_t)s_data.circleVBOBase;
        if (dataSize > 0)
        {
            s_data.circleVBO->setData(s_data.circleVBOBase, dataSize);

            s_data.circleVAO->bind();
            s_data.circleShader->bind();
            RendererAPI::drawIndexed(s_data.currentCircleIndexCount, s_data.circleVAO->getIndexBuffer()->getIndexType());

            ++s_data.statistics.drawCalls;
        }
    }

    static void beginLineBatch()
    {
        s_data.currentLineVertexCount = 0;
        s_data.lineVBOPtr = s_data.lineVBOBase;
    }

    static void endLineBatch()
    {
        size_t dataSize = (uintptr_t)s_data.lineVBOPtr - (uintptr_t)s_data.lineVBOBase;
        if (dataSize > 0)
        {
            s_data.lineVBO->setData(s_data.lineVBOBase, dataSize);

            s_data.lineVAO->bind();
            s_data.lineShader->bind();
            RendererAPI::draw(s_data.currentLineVertexCount, RendererAPI::PrimitiveType::Lines);

            ++s_data.statistics.drawCalls;
        }
    }

    void init()
    {
        JNG_PROFILE_FUNCTION();

        std::filesystem::path assetsDir = Engine::get().getProperties().assetsDirectory;

        s_data.cameraUBO = makeRef<UniformBuffer>(sizeof(glm::mat4));
        s_data.cameraUBO->bind(0);

        Texture::Properties props{
            TextureFormat::RGBA8,
            1, 1
        };
        s_data.whiteTexture = makeRef<Texture>(props);
        u32 whiteTextureData = 0xffffffff;
        s_data.whiteTexture->setData(&whiteTextureData, sizeof(u32));
        s_data.textureSlots[0] = s_data.whiteTexture;

        s_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        u16* quadIndices = new u16[RenderData::MaxQuadIndicesPerBatch];
        for (u16 i = 0, offset = 0; i < s_data.MaxQuadIndicesPerBatch; i += s_data.QuadIndexCount, offset += s_data.QuadVertexCount)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 2;
            quadIndices[i + 2] = offset + 1;

            quadIndices[i + 3] = offset + 0;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 2;
        }
        auto quadIBO = makeRef<IndexBuffer>(quadIndices, RenderData::MaxQuadIndicesPerBatch, RendererAPI::IndexType::U16);
        delete[] quadIndices;

        // Text
        s_data.textShader = makeRef<Shader>(
            assetsDir / "shaders/text_vertex.glsl",
            assetsDir / "shaders/text_fragment.glsl");
        s_data.textVBO = makeRef<VertexBuffer>(RenderData::MaxQuadVerticesPerBatch * sizeof(TextVertex));

        VertexLayout textVertexLayout = {
            { LayoutElement::DataType::Float3,  "a_Position" },
            { LayoutElement::DataType::Float2,  "a_TexCoord" },
            { LayoutElement::DataType::UInt4x8, "a_Color", true, true },
            { LayoutElement::DataType::Int,     "a_EntityID", false }
        };
        s_data.textVAO = makeRef<VertexArray>(s_data.textVBO, textVertexLayout);
        s_data.textVBOBase = new TextVertex[s_data.MaxQuadVerticesPerBatch];
        s_data.textVAO->setIndexBuffer(quadIBO);

        // Quad
        s_data.quadShader = makeRef<Shader>(
            (assetsDir / "shaders/quad_vertex.glsl").string(),
            (assetsDir / "shaders/quad_fragment.glsl").string()
        );
        s_data.quadVBO = makeRef<VertexBuffer>(RenderData::MaxQuadVerticesPerBatch * sizeof(QuadVertex));

        VertexLayout quadVertexLayout = {
            { LayoutElement::DataType::Float3,  "a_Position" },
            { LayoutElement::DataType::Float2,  "a_TexCoord" },
            { LayoutElement::DataType::UInt4x8, "a_Color", true, true },
            { LayoutElement::DataType::UInt,    "a_TexIndex", false },
            { LayoutElement::DataType::Int,     "a_EntityID", false }
        };
        s_data.quadVAO = makeRef<VertexArray>(s_data.quadVBO, quadVertexLayout);
        s_data.quadVBOBase = new QuadVertex[s_data.MaxQuadVerticesPerBatch];
        s_data.quadVAO->setIndexBuffer(quadIBO);

        // Circle
        s_data.circleShader = makeRef<Shader>(
            (assetsDir / "shaders/circle_vertex.glsl").string(),
            (assetsDir / "shaders/circle_fragment.glsl").string()
        );
        s_data.circleVBO = makeRef<VertexBuffer>(RenderData::MaxQuadVerticesPerBatch * sizeof(CircleVertex));

        VertexLayout circleVertexLayout = {
            { LayoutElement::DataType::Float3,  "a_Position" },
            { LayoutElement::DataType::Float2,  "a_LocalPosition" },
            { LayoutElement::DataType::Float,   "a_Thickness" },
            { LayoutElement::DataType::Float,   "a_Fade" },
            { LayoutElement::DataType::UInt4x8, "a_Color", true, true },
            { LayoutElement::DataType::Int,     "a_EntityID", false }
        };
        s_data.circleVAO = makeRef<VertexArray>(s_data.circleVBO, circleVertexLayout);
        s_data.circleVBOBase = new CircleVertex[s_data.MaxQuadVerticesPerBatch];
        s_data.circleVAO->setIndexBuffer(quadIBO);

        // Line
        s_data.lineShader = makeRef<Shader>(
            (assetsDir / "shaders/line_vertex.glsl").string(),
            (assetsDir / "shaders/line_fragment.glsl").string()
        );
        s_data.lineVBO = makeRef<VertexBuffer>(RenderData::MaxLineVerticesPerBatch * sizeof(LineVertex));

        VertexLayout lineVertexLayout = {
            { LayoutElement::DataType::Float3,  "a_Position" },
            { LayoutElement::DataType::UInt4x8, "a_Color", true, true }
        };
        s_data.lineVAO = makeRef<VertexArray>(s_data.lineVBO, lineVertexLayout);
        s_data.lineVBOBase = new LineVertex[s_data.MaxLineVerticesPerBatch];
    }

    void shutdown()
    {
        delete[] s_data.textVBOBase;
        delete[] s_data.quadVBOBase;
        delete[] s_data.circleVBOBase;
        delete[] s_data.lineVBOBase;
    }

    void beginScene(const glm::mat4& viewProjection)
    {
        JNG_PROFILE_FUNCTION();

        s_data.cameraUBO->setData(glm::value_ptr(viewProjection), sizeof(glm::mat4));

        s_data.statistics.drawCalls = 0;
        s_data.statistics.quadCount = 0;
        s_data.statistics.circleCount = 0;
        s_data.statistics.lineCount = 0;
        
        beginTextBatch();
        beginQuadBatch();
        beginCircleBatch();
        beginLineBatch();
    }

    void endScene()
    {
        JNG_PROFILE_FUNCTION();

        endTextBatch();
        endQuadBatch();
        endCircleBatch();
        endLineBatch();
    }

    void drawText(const glm::mat4& transform, const std::string& text, Ref<Font> font, const glm::vec4& color)
    {
        JNG_PROFILE_FUNCTION();

        const auto& geometry = font->getFontData()->geometry;
        const auto& metrics = geometry.getMetrics();
        auto fontAtlas = font->getAtlasTexture();
        s_data.fontAtlasTexture = fontAtlas;

        double x = 0;
        double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
        double y = 0;
        for (size_t i = 0; i < text.size(); i++)
        {
            char character = text[i];
            auto glyph = geometry.getGlyph(character);
            if (!glyph)
                glyph = geometry.getGlyph('?');

            double al, ab, ar, at;
            glyph->getQuadAtlasBounds(al, ab, ar, at);
            glm::vec2 texCoordMin{ (float)al, (float)ab };
            glm::vec2 texCoordMax{ (float)ar, (float)at };
            float texelWidth = 1.f / fontAtlas->getProperties().width;
            float texelHeight = 1.f / fontAtlas->getProperties().height;
            texCoordMin *= glm::vec2{ texelWidth, texelHeight };
            texCoordMax *= glm::vec2{ texelWidth, texelHeight };

            double pl, pb, pr, pt;
            glyph->getQuadPlaneBounds(pl, pb, pr, pt);
            glm::vec2 quadMin{ (float)pl, (float)pb };
            glm::vec2 quadMax{ (float)pr, (float)pt };
            quadMin *= fsScale; quadMax *= fsScale;
            quadMin += glm::vec2{ x, y }; quadMax += glm::vec2{ x, y };

            // render
            s_data.textVBOPtr->position = transform * glm::vec4(quadMin, 0.f, 1.f);
            s_data.textVBOPtr->texCoord = texCoordMin;
            s_data.textVBOPtr->color = glm::packUnorm4x8(color);
            s_data.textVBOPtr->entityID = -1;
            ++s_data.textVBOPtr;

            s_data.textVBOPtr->position = transform * glm::vec4(quadMin.x, quadMax.y, 0.f, 1.f);
            s_data.textVBOPtr->texCoord = { texCoordMin.x, texCoordMax.y };
            s_data.textVBOPtr->color = glm::packUnorm4x8(color);
            s_data.textVBOPtr->entityID = -1;
            ++s_data.textVBOPtr;

            s_data.textVBOPtr->position = transform * glm::vec4(quadMax, 0.f, 1.f);
            s_data.textVBOPtr->texCoord = texCoordMax;
            s_data.textVBOPtr->color = glm::packUnorm4x8(color);
            s_data.textVBOPtr->entityID = -1;
            ++s_data.textVBOPtr;

            s_data.textVBOPtr->position = transform * glm::vec4(quadMax.x, quadMin.y, 0.f, 1.f);
            s_data.textVBOPtr->texCoord = { texCoordMax.x, texCoordMin.y };
            s_data.textVBOPtr->color = glm::packUnorm4x8(color);
            s_data.textVBOPtr->entityID = -1;
            ++s_data.textVBOPtr;

            s_data.currentTextIndexCount += RenderData::QuadIndexCount;

            ++s_data.statistics.quadCount;

            double advance = glyph->getAdvance();
            geometry.getAdvance(advance, character, text[i + 1]);
            x += fsScale * advance /*+ textParams.Kerning*/;
        }
    }

    void drawSprite(const glm::mat4& transform, const SpriteRendererComponent& src, s32 entityID)
    {
        glm::vec3 quadVertexPositions[RenderData::QuadVertexCount];

        for (u16 i = 0; i < RenderData::QuadVertexCount; ++i)
            quadVertexPositions[i] = transform * s_data.quadVertexPositions[i];

        constexpr glm::vec2 texCoords[RenderData::QuadVertexCount] = {
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

    void drawQuad(const DrawQuadProperties& properties)
    {
        JNG_PROFILE_FUNCTION();

        if (s_data.currentQuadIndexCount >= RenderData::MaxQuadIndicesPerBatch)
        {
            endQuadBatch();
            beginQuadBatch();
        }

        u8 textureIndex = static_cast<u8>(-1);
        for (u8 i = 0; i < s_data.textureSlotIndex; ++i)
            if (s_data.textureSlots[i]->getID() == properties.texture->getID())
            {
                textureIndex = i;
                break;
            }

        if (textureIndex == static_cast<u8>(-1))
        {
            s_data.textureSlots[s_data.textureSlotIndex] = properties.texture;
            textureIndex = s_data.textureSlotIndex++;
        }

        for (u16 i = 0; i < RenderData::QuadVertexCount; ++i)
        {
            s_data.quadVBOPtr->position = properties.vertexPositions[i];
            s_data.quadVBOPtr->texCoord = properties.textureCoords[i];
            s_data.quadVBOPtr->color = properties.color;
            s_data.quadVBOPtr->texIndex = textureIndex;
            s_data.quadVBOPtr->entityID = properties.entityID;
            ++s_data.quadVBOPtr;
        }

        s_data.currentQuadIndexCount += RenderData::QuadIndexCount;

        ++s_data.statistics.quadCount;
    }

    void drawCircle(const glm::mat4& transform, const CircleRendererComponent& crc, s32 entityID)
    {
        glm::vec3 circleVertexPositions[RenderData::QuadVertexCount];
        glm::vec2 circleVertexLocalPositions[RenderData::QuadVertexCount];

        for (u16 i = 0; i < RenderData::QuadVertexCount; ++i)
        {
            circleVertexPositions[i] = transform * s_data.quadVertexPositions[i];
            circleVertexLocalPositions[i] = s_data.quadVertexPositions[i] * 2.f;
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

    void drawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade)
    {
        glm::vec3 circleVertexPositions[RenderData::QuadVertexCount];
        glm::vec2 circleVertexLocalPositions[RenderData::QuadVertexCount];

        for (u16 i = 0; i < RenderData::QuadVertexCount; ++i)
        {
            circleVertexPositions[i] = transform * s_data.quadVertexPositions[i];
            circleVertexLocalPositions[i] = s_data.quadVertexPositions[i] * 2.f;
        }

        const DrawCircleProperties properties{
            circleVertexPositions,
            circleVertexLocalPositions,
            thickness,
            fade,
            glm::packUnorm4x8(color),
            -1
        };
        drawCircle(properties);
    }

    void drawCircle(const DrawCircleProperties& properties)
    {
        JNG_PROFILE_FUNCTION();

        if (s_data.currentCircleIndexCount >= RenderData::MaxQuadIndicesPerBatch)
        {
            endCircleBatch();
            beginCircleBatch();
        }

        for (u16 i = 0; i < RenderData::QuadVertexCount; ++i)
        {
            s_data.circleVBOPtr->position = properties.vertexPositions[i];
            s_data.circleVBOPtr->localPosition = properties.localVertexPositions[i];
            s_data.circleVBOPtr->thickness = properties.thickness;
            s_data.circleVBOPtr->fade = properties.fade;
            s_data.circleVBOPtr->color = properties.color;
            s_data.circleVBOPtr->entityID = properties.entityID;
            ++s_data.circleVBOPtr;
        }

        s_data.currentCircleIndexCount += RenderData::QuadIndexCount;

        ++s_data.statistics.circleCount;
    }

    void drawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
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

    void drawRect(const glm::mat4& transform, const glm::vec4& color)
    {
        glm::vec3 p0 = transform * s_data.quadVertexPositions[0];
        glm::vec3 p1 = transform * s_data.quadVertexPositions[1];
        glm::vec3 p2 = transform * s_data.quadVertexPositions[2];
        glm::vec3 p3 = transform * s_data.quadVertexPositions[3];

        drawLine(p0, p1, color);
        drawLine(p1, p2, color);
        drawLine(p2, p3, color);
        drawLine(p3, p0, color);
    }

    void fillQuad(glm::vec3 position, glm::vec2 size, const glm::vec4& color)
    {
        fillQuad(position, size, s_data.whiteTexture, color);
    }

    void fillQuad(glm::vec3 position, glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color)
    {
        glm::vec3 quadVertexPositions[RenderData::QuadVertexCount];

        quadVertexPositions[0] = { position.x,          position.y,          position.z };
        quadVertexPositions[1] = { position.x + size.x, position.y,          position.z };
        quadVertexPositions[2] = { position.x + size.x, position.y + size.y, position.z };
        quadVertexPositions[3] = { position.x,          position.y + size.y, position.z };

        constexpr glm::vec2 texCoords[RenderData::QuadVertexCount] = {
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

    void fillQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        fillQuad(transform, s_data.whiteTexture, color);
    }

    void fillQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& color)
    {
        glm::vec3 quadVertexPositions[RenderData::QuadVertexCount];

        for (u16 i = 0; i < RenderData::QuadVertexCount; ++i)
            quadVertexPositions[i] = transform * s_data.quadVertexPositions[i];

        constexpr glm::vec2 texCoords[RenderData::QuadVertexCount] = {
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

    const Statistics& getStatistics()
    {
        return s_data.statistics;
    }

} // namespace jng::Renderer2D
