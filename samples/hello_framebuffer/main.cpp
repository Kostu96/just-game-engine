/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define JNG_DECLARE_MAIN
#include <jng/jng.hpp>

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

const uint32_t indices[]{
    0, 2, 1,
    0, 3, 2
};

class SampleLayer :
    public jng::Layer
{
public:
    SampleLayer() :
        m_framebuffer{ jng::Framebuffer::create({WindowWidth, WindowHeight}) },
        m_texture{ jng::Texture::create("assets/hello_framebuffer/textures/test.png")},
        m_shader{ jng::Shader::create("assets/hello_framebuffer/shaders/vertex.glsl", "assets/hello_framebuffer/shaders/fragment.glsl") },
        m_UBO{ jng::UniformBuffer::create(sizeof(glm::mat4)) },
        m_VBO{ jng::VertexBuffer::create(vertices, sizeof(vertices)) },
        m_IBO{ jng::IndexBuffer::create(indices, sizeof(indices)) },
        m_VAO{ jng::VertexArray::create(m_VBO, LAYOUT, m_shader) },
        m_camera{ -2.f, 2.f, -1.5f, 1.5f }
    {
        // NOTE: These can be bound once at the begining because they're only one used.
        m_shader->bind();
        m_UBO->bind(0);
        m_VAO->setIndexBuffer(m_IBO);
        m_VAO->bind();

        m_UBO->setData(glm::value_ptr(m_camera.getVP()), sizeof(glm::mat4));
    }

    void onUpdate(float /*dt*/) override
    {
        // offscreen render
        m_framebuffer->bind();
        jng::RendererAPI::clear({ 0.1f, 0.1f, 0.2f });

        m_texture->bind(0);
        jng::RendererAPI::drawIndexed(6);

        // onscreen render
        m_framebuffer->unbind();
        jng::RendererAPI::clear({ 0.2f, 0.2f, 0.4f });

        ImGui::Begin("Framebuffer");
        ImGui::Image(m_framebuffer->getColorAttachmentHandle(), { 400, 300 });
        ImGui::End();
    }
private:
    static const jng::VertexLayout LAYOUT;
    jng::Ref<jng::Framebuffer> m_framebuffer;
    jng::Ref<jng::Texture> m_texture;
    jng::Ref<jng::Shader> m_shader;
    jng::Ref<jng::UniformBuffer> m_UBO;
    jng::Ref<jng::VertexBuffer> m_VBO;
    jng::Ref<jng::IndexBuffer> m_IBO;
    jng::Ref<jng::VertexArray> m_VAO;
    jng::OrthographicCamera m_camera;
};

const jng::VertexLayout SampleLayer::LAYOUT{
    { jng::LayoutElement::DataType::Float2, "a_Position" },
    { jng::LayoutElement::DataType::Float2, "a_TexCoord" }
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
