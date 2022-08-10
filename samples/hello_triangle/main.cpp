/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define JNG_DECLARE_MAIN
#include <jng/jng.hpp>

static constexpr unsigned int WindowWidth = 800;
static constexpr unsigned int WindowHeight = 600;

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
        m_cameraUBO{ jng::makeRef<jng::UniformBuffer>(sizeof(glm::mat4)) },
        m_VBO{ jng::makeRef<jng::VertexBuffer>(vertices, sizeof(vertices)) },
        m_VAO{ jng::VertexArray::create(m_VBO, LAYOUT, m_shader) }
    {
        // NOTE: These can be bound once at the begining because they're only one used.
        m_shader->bind();
        m_cameraUBO->bind(0);
        m_VAO->bind();

        m_camera.setViewportSize(WindowWidth, WindowHeight);
        m_camera.setOrthographicSize(4.f);
        m_cameraUBO->setData(glm::value_ptr(m_camera.getVP(glm::mat4{ 1.f })), sizeof(glm::mat4));
    }

    void onUpdate(float /*dt*/) override
    {
        jng::RendererAPI::clear({ 0.1f, 0.1f, 0.2f });

        jng::RendererAPI::draw(3);
    }

    void onEvent(jng::Event& event) override
    {
        jng::EventDispatcher dispatcher(event);
        dispatcher.dispatch<jng::WindowResizeEvent>(JNG_BIND_EVENT_FUNC(SampleLayer::onWindowResize));
    }
private:
    bool onWindowResize(jng::WindowResizeEvent& event)
    {
        m_camera.setViewportSize(static_cast<float>(event.getWidth()), static_cast<float>(event.getHeight()));
        m_cameraUBO->setData(glm::value_ptr(m_camera.getVP(glm::mat4{ 1.f })), sizeof(glm::mat4), 0);
        return false;
    }

    static const jng::VertexLayout LAYOUT;
    jng::Ref<jng::Shader> m_shader;
    jng::Camera m_camera;
    jng::Ref<jng::UniformBuffer> m_cameraUBO;
    jng::Ref<jng::VertexBuffer> m_VBO;
    jng::Ref<jng::VertexArray> m_VAO;
};

const jng::VertexLayout SampleLayer::LAYOUT{ { jng::LayoutElement::DataType::Float2, "a_Position" } };

class App :
    public jng::Engine
{
public:
    App() :
        Engine({
            "Hello Triangle!",
            WindowWidth,
            WindowHeight,
            RendererType::None,
            "assets/hello_triangle"
        })
    {
        getLayerStack().pushLayer(new SampleLayer{});
    }
};

jng::Engine* createApp() {
    return new App{};
}
