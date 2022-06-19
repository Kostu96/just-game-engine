/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

#include "renderer/camera.hpp"

namespace jng {

    class VertexArray;
    class RendererAPI;
    class Texture;

    class Renderer2D
    {
    public:
        struct Properties
        {
            const glm::vec2* quadVertexPositions;
            const glm::vec2* textureCoords;
            const Ref<Texture>& texture;
            const glm::vec4& color;
        };

        static void init(RendererBackend api);
        static void shutdown();

        static void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
        static void clear(glm::vec3 color);

        static void beginScene(const Camera& camera);
        static void endScene();

        static void fillQuad(const Properties& properties);
        static void fillQuad(glm::vec2 position, glm::vec2 size, const glm::vec4& color);
        static void fillQuad(glm::vec2 position, glm::vec2 size, float rotation, const glm::vec4& color);
        static void fillQuad(glm::vec2 position, glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color = { 1.f, 1.f, 1.f, 1.f });
        static void fillQuad(glm::vec2 position, glm::vec2 size, float rotation, const Ref<Texture>& texture, const glm::vec4& color = { 1.f, 1.f, 1.f, 1.f });

        struct Statistics
        {
            uint32 drawCalls = 0;
            uint32 quadCount = 0;
        };

        static const Statistics& getStatistics();
    private:
        // TODO: NonCopyable from cpp-common-lib
        Renderer2D() = delete;
        Renderer2D(const Renderer2D&) = delete;
        Renderer2D& operator=(const Renderer2D&) = delete;

        static void beginBatch();
        static void endBatch();

        static Scope<RendererAPI> s_rendererAPI;
    };

} // namespace jng
