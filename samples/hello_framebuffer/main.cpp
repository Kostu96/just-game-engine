/*
 * Copyright (C) 2022-2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include <jng/core/engine.hpp>
#include <jng/core/entry_point.hpp>
#include <jng/core/event.hpp>
#include <jng/core/layer.hpp>
#include <jng/renderer/buffers.hpp>
#include <jng/renderer/framebuffer.hpp>
#include <jng/renderer/shader.hpp>
#include <jng/renderer/vertex_array.hpp>
#include <jng/scene/camera.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

static constexpr unsigned int WindowWidth = 800;
static constexpr unsigned int WindowHeight = 600;

struct Vertex {
    glm::vec2 position;
    glm::vec2 texCoord;
};

const Vertex vertices[]{
    { { -1.f, -1.f }, { 0.f, 0.f } },
    { {  1.f, -1.f }, { 1.f, 0.f } },
    { {  1.f,  1.f }, { 1.f, 1.f } },
    { { -1.f,  1.f }, { 0.f, 1.f } }
};

const jng::u8 indices[]{
    0, 2, 1,
    0, 3, 2
};

class SampleLayer :
    public jng::Layer
{
public:
    SampleLayer() :
        m_framebuffer{ jng::makeRef<jng::Framebuffer>(jng::Framebuffer::Properties{
            .width = WindowWidth, .height = WindowHeight,
            .attachmentsProperties = {
                jng::Texture::Properties { .format = jng::Texture::Format::RGBA8 },
                jng::Texture::Properties { .format = jng::Texture::Format::Depth24Stencil8 }
        }}) },
        m_texture{ jng::makeRef<jng::Texture>("assets/hello_framebuffer/textures/test.png") },
        m_shader{ jng::makeRef<jng::Shader>("assets/hello_framebuffer/shaders/vertex.glsl", "assets/hello_framebuffer/shaders/fragment.glsl") },
        m_cameraUBO{ jng::makeRef<jng::UniformBuffer>(sizeof(glm::mat4)) },
        m_VBO{ jng::makeRef<jng::VertexBuffer>(vertices, sizeof(vertices)) },
        m_IBO{ jng::makeRef<jng::IndexBuffer>(indices, (jng::u32)(sizeof(indices) / sizeof(jng::u8)), jng::RendererAPI::IndexType::U8) },
        m_VAO{ jng::makeRef<jng::VertexArray>(m_VBO, LAYOUT) }
    {
        // NOTE: These can be bound once at the begining because they're only one used.
        m_shader->bind();
        m_cameraUBO->bind(0);
        m_VAO->setIndexBuffer(m_IBO);
        m_VAO->bind();

        m_camera.setViewportSize(400, 300);
        m_camera.setOrthographicSize(4.f);
        m_cameraUBO->setData(glm::value_ptr(m_camera.getVP(glm::mat4{ 1.f })), sizeof(glm::mat4));
    }

    void onUpdate(float /*dt*/) override
    {
        // offscreen render
        m_framebuffer->bind();
        jng::RendererAPI::clear({ 0.1f, 0.1f, 0.2f });

        m_texture->bind(0);
        jng::RendererAPI::drawIndexed(m_VAO->getIndexBuffer()->getCount(), m_VAO->getIndexBuffer()->getIndexType());

        // onscreen render
        m_framebuffer->unbind();
        jng::RendererAPI::clear({ 0.2f, 0.2f, 0.4f });

        m_texture->bind(0);
        jng::RendererAPI::drawIndexed(m_VAO->getIndexBuffer()->getCount(), m_VAO->getIndexBuffer()->getIndexType());
    }

    void onImGuiUpdate() override
    {
        ImGui::Begin("Framebuffer");
        ImGui::Image(m_framebuffer->getAttachments()[0]->getRendererID(), {400, 300});
        ImGui::End();
    }
private:
    static const jng::VertexLayout LAYOUT;
    jng::Ref<jng::Framebuffer> m_framebuffer;
    jng::Ref<jng::Texture> m_texture;
    jng::Ref<jng::Shader> m_shader;
    jng::Camera m_camera;
    jng::Ref<jng::UniformBuffer> m_cameraUBO;
    jng::Ref<jng::VertexBuffer> m_VBO;
    jng::Ref<jng::IndexBuffer> m_IBO;
    jng::Ref<jng::VertexArray> m_VAO;
};

const jng::VertexLayout SampleLayer::LAYOUT{
    { jng::LayoutElement::DataType::Float2 },
    { jng::LayoutElement::DataType::Float2 }
};

class App :
    public jng::Engine
{
public:
    App() :
        Engine({
            "Hello Framebuffer!",
            WindowWidth,
            WindowHeight,
            RendererType::None,
            "assets/hello_framebuffer"
        })
    {
        getLayerStack().pushLayer(new SampleLayer{});
    }
};

jng::Engine* createApp() {
    return new App{};
}
