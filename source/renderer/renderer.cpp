/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/renderer.hpp"

#include "core/base_internal.hpp"
#include "renderer/direct3d/renderer_impl_d3d.hpp"
#include "renderer/opengl/renderer_impl_ogl.hpp"

#include <array>
#include <glm/gtc/matrix_transform.hpp>

namespace jng {

    void Renderer::init(RenderAPI api)
    {
        JNG_PROFILE_FUNCTION();

        switch (api)
        {
        case RenderAPI::Direct3D: s_implementation = makeScope<Direct3DRendererImpl>();
        case RenderAPI::OpenGL: s_implementation = makeScope<OpenGLRendererImpl>();
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
        }

    }

    void Renderer::shutdown()
    {
        delete[] s_data.quadVBOBase;
    }

    void Renderer::setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        JNG_PROFILE_FUNCTION();

        s_rendererAPI->setViewport(x, y, width, height);
    }

    void Renderer::clear(glm::vec3 color)
    {
        JNG_PROFILE_FUNCTION();

        s_rendererAPI->clear(color);
    }

    void Renderer::beginScene(const Camera& camera)
    {
        JNG_PROFILE_FUNCTION();

        s_data.VP = camera.getVP();
        s_data.shader->set("u_VP", s_data.VP);

        s_data.statistics.drawCalls = 0;
        s_data.statistics.quadCount = 0;
        beginBatch();
    }

    void Renderer::endScene()
    {
        JNG_PROFILE_FUNCTION();

        endBatch();
    }

    void Renderer::fillQuad(const Properties& properties)
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

    void Renderer::fillQuad(glm::vec2 position, glm::vec2 size, const glm::vec4& color)
    {
        fillQuad(position, size, s_data.whiteTexture, color);
    }

    void Renderer::fillQuad(glm::vec2 position, glm::vec2 size, float rotation, const glm::vec4& color)
    {
        fillQuad(position, size, rotation, s_data.whiteTexture, color);
    }

    void Renderer::fillQuad(glm::vec2 position, glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color)
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

    void Renderer::fillQuad(glm::vec2 position, glm::vec2 size, float rotation, const Ref<Texture>& texture, const glm::vec4& color)
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

    const Renderer::Statistics& Renderer::getStatistics()
    {
        return s_data.statistics;
    }

    void Renderer::beginBatch()
    {
        s_data.currentQuadIndexCount = 0;
        s_data.quadVBOPtr = s_data.quadVBOBase;
        s_data.textureSlotIndex = 1;
    }

    void Renderer::endBatch()
    {
        for (uint32 i = 1; i < s_data.textureSlotIndex; ++i)
            s_data.textureSlots[i]->bind(i);

        size_t dataSize = static_cast<size_t>(
            reinterpret_cast<uint8*>(s_data.quadVBOPtr) -
            reinterpret_cast<uint8*>(s_data.quadVBOBase)
        );
        s_data.quadVBO->setData(s_data.quadVBOBase, dataSize);

        s_data.quadVAO->bind();
        s_rendererAPI->drawIndexed(s_data.currentQuadIndexCount);

        ++s_data.statistics.drawCalls;
    }

    Scope<RendererAPI> Renderer::s_rendererAPI = RendererAPI::create();

} // namespace k2d
