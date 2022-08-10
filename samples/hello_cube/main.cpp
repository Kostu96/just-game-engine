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
    glm::vec3 position;
    glm::vec2 texCoord;
};

const Vertex vertices[]{
    {{ -1.f, -1.f,  1.f }, { 0.f, 0.f }}, // 0
    {{  1.f, -1.f,  1.f }, { 1.f, 0.f }}, // 1
    {{  1.f,  1.f,  1.f }, { 1.f, 1.f }}, // 2
    {{ -1.f,  1.f,  1.f }, { 0.f, 1.f }}, // 3
    {{  1.f, -1.f, -1.f }, { 0.f, 0.f }}, // 4
    {{  1.f,  1.f, -1.f }, { 0.f, 1.f }}, // 5
    {{ -1.f,  1.f, -1.f }, { 0.f, 0.f }}, // 6
    {{ -1.f,  1.f,  1.f }, { 1.f, 0.f }}, // 7
    {{ -1.f, -1.f,  1.f }, { 1.f, 1.f }}, // 8
    {{ -1.f, -1.f, -1.f }, { 0.f, 1.f }}, // 9
    {{ -1.f, -1.f, -1.f }, { 1.f, 0.f }}, // 10
    {{ -1.f,  1.f, -1.f }, { 1.f, 1.f }}, // 11
    {{ -1.f, -1.f,  1.f }, { 0.f, 0.f }}, // 12
    {{ -1.f,  1.f,  1.f }, { 0.f, 1.f }}  // 13
};

const jng::uint32 indices[]{
    // front
    0, 3, 2,
    2, 1, 0,
    // right
    1, 2, 5,
    5, 4, 1,
    // top
    2, 7, 6,
    6, 5, 2,
    // bottom
    8, 1, 4,
    4, 9, 8,
    // back
    4, 5, 11,
    11, 10, 4,
    // left
    10, 11, 3,
    3, 0, 10,
};

class SampleLayer :
    public jng::Layer
{
public:
    SampleLayer() :
        m_shader{ jng::makeRef<jng::Shader>("assets/hello_cube/shaders/vertex.glsl", "assets/hello_cube/shaders/fragment.glsl") },
        m_cameraUBO{ jng::makeRef<jng::UniformBuffer>(sizeof(glm::mat4)) },
        m_modelUBO{ jng::makeRef<jng::UniformBuffer>(sizeof(glm::mat4)) },
        m_VBO{ jng::makeRef<jng::VertexBuffer>(vertices, sizeof(vertices)) },
        m_IBO{ jng::makeRef<jng::IndexBuffer>(indices, (jng::uint32)(sizeof(indices) / sizeof(jng::uint32))) },
        m_VAO{ jng::makeRef<jng::VertexArray>(m_VBO, LAYOUT) },
        m_texture{ jng::makeRef<jng::Texture>("assets/hello_cube/textures/wall_base_color.jpg") },
        m_model{ 1.f }
    {
        m_VAO->setIndexBuffer(m_IBO);

        // NOTE: These can be bound once at the begining because they're only one used.
        m_shader->bind();
        m_cameraUBO->bind(0);
        m_modelUBO->bind(1);
        m_VAO->bind();
        m_texture->bind(0);

        m_camera.setViewportSize(WindowWidth, WindowHeight);
        m_camera.setPerspective(45.f);
        m_cameraUBO->setData(glm::value_ptr(m_camera.getVP(glm::mat4{ 1.f })), sizeof(glm::mat4), 0);
        m_model = glm::translate(m_model, glm::vec3{ 0.f, 0.f, -5.f });
    }

    void onUpdate(float dt) override
    {
        jng::RendererAPI::clear({ 0.1f, 0.1f, 0.2f });

        m_model = glm::rotate(m_model, dt, glm::vec3{ 1.f, 0.7f, 0.f });
        m_modelUBO->setData(glm::value_ptr(m_model), sizeof(glm::mat4), 0);

        jng::RendererAPI::drawIndexed(m_VAO->getIndexBuffer()->getCount());
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
    jng::Ref<jng::UniformBuffer> m_modelUBO;
    jng::Ref<jng::VertexBuffer> m_VBO;
    jng::Ref<jng::IndexBuffer> m_IBO;
    jng::Ref<jng::VertexArray> m_VAO;
    jng::Ref<jng::Texture> m_texture;
    glm::mat4 m_model;
};

const jng::VertexLayout SampleLayer::LAYOUT{
    { jng::LayoutElement::DataType::Float3, "a_Position" },
    { jng::LayoutElement::DataType::Float2, "a_TexCoord" }
};

class App :
    public jng::Engine
{
public:
    App() :
        Engine({
            "Hello Cube!",
            WindowWidth,
            WindowHeight,
            RendererType::None,
            "assets/hello_cube"
        })
    {
        getLayerStack().pushLayer(new SampleLayer{});
    }
};

jng::Engine* createApp() {
    return new App{};
}
