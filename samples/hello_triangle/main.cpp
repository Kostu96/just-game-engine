/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define JNG_DECLARE_MAIN
#include <jng/jng.hpp>

const glm::vec2 vertices[]{
    { -1.f, -1.f },
    {  0.f,  1.f },
    {  1.f, -1.f }
};

class SampleLayer :
    public jng::Layer
{
public:
    SampleLayer() :
        m_shader{ jng::Shader::create("assets/hello_triangle/shaders/vertex.glsl", "assets/hello_triangle/shaders/fragment.glsl") },
        m_UBO{ jng::UniformBuffer::create(sizeof(glm::mat4)) },
        m_VBO{ jng::VertexBuffer::create(vertices, sizeof(vertices)) },
        m_VAO{ jng::VertexArray::create(m_VBO, LAYOUT, m_shader) },
        m_camera{ -2.f, 2.f, -1.5f, 1.5f }
    {
        // NOTE: These can be bound once at the begining because they're only one used.
        m_shader->bind();
        m_UBO->bind(0);
        m_VAO->bind();

        m_UBO->setData(glm::value_ptr(m_camera.getVP()), sizeof(glm::mat4));
    }

    void onUpdate(float /*dt*/) override
    {
        jng::RendererAPI::clear({ 0.1f, 0.1f, 0.2f });

        jng::RendererAPI::draw(3);
    }
private:
    static const jng::VertexLayout LAYOUT;
    jng::Ref<jng::Shader> m_shader;
    jng::Ref<jng::UniformBuffer> m_UBO;
    jng::Ref<jng::VertexBuffer> m_VBO;
    jng::Ref<jng::VertexArray> m_VAO;
    jng::OrthographicCamera m_camera;
};

const jng::VertexLayout SampleLayer::LAYOUT{ { jng::LayoutElement::DataType::Float2, "a_Position" } };

class App :
    public jng::Engine
{
public:
    static constexpr unsigned int WindowWidth = 800;
    static constexpr unsigned int WindowHeight = 600;

    App() :
        Engine({
            "Hello Triangle!",
            WindowWidth,
            WindowHeight,
            RendererType::None
        })
    {
        getLayerStack().pushLayer(new SampleLayer{});
    }
};

jng::Engine* createApp() {
    return new App{};
}
