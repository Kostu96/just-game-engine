/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define JNG_DECLARE_MAIN
#include <jng/jng.hpp>

const char* vert_shader = R"(
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(location = 0) out vec3 v_Color;
layout(location = 1) out vec2 v_TexCoord;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_VP;
};

layout(std140, binding = 1) uniform Object
{
	mat4 u_Model;
};

void main()
{
    v_Color = vec3(a_Position.x + 1.0, a_Position.y + 1.0, a_Position.z + 1.0);
    v_TexCoord = a_TexCoord;
    gl_Position = u_VP * u_Model * vec4(a_Position, 1.0);
}
)";

const char* frag_shader = R"(
#version 450 core

layout(location = 0) in vec3 v_Color;
layout(location = 1) in vec2 v_TexCoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D u_Texture;

void main()
{
    fragColor = vec4(v_Color, 1.0) * texture(u_Texture, v_TexCoord);
}
)";

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
        m_shader{ jng::Shader::create(vert_shader, frag_shader) },
        m_cameraUBO{ jng::UniformBuffer::create(sizeof(glm::mat4)) },
        m_modelUBO{ jng::UniformBuffer::create(sizeof(glm::mat4)) },
        m_VBO{ jng::VertexBuffer::create(vertices, sizeof(vertices)) },
        m_IBO{ jng::IndexBuffer::create(indices, sizeof(indices)) },
        m_VAO{ jng::VertexArray::create(m_VBO, LAYOUT, m_shader) },
        m_texture{ jng::Texture::create("assets/wall_base_color.jpg") },
        m_camera{ 45.f, 4.f/3.f, 0.1f, 100.f },
        m_model{ 1.f }
    {
        m_VAO->setIndexBuffer(m_IBO);

        // NOTE: These can be bound once at the begining because they're only one used.
        m_shader->bind();
        m_cameraUBO->bind(0);
        m_modelUBO->bind(1);
        m_VAO->bind();
        m_texture->bind(0);

        m_cameraUBO->setData(glm::value_ptr(m_camera.getVP()), sizeof(glm::mat4), 0);

        m_model = glm::translate(m_model, glm::vec3{ 0.f, 0.f, -5.f });
    }

    void onUpdate(float dt) override
    {
        jng::RendererAPI::clear({ 0.1f, 0.1f, 0.2f });

        m_model = glm::rotate(m_model, dt, glm::vec3{ 1.f, 0.7f, 0.f });
        m_modelUBO->setData(glm::value_ptr(m_model), sizeof(glm::mat4), 0);

        jng::RendererAPI::drawIndexed(m_VAO);
    }

    void onEvent(jng::Event& event) override
    {
        jng::EventDispatcher dispatcher(event);
        dispatcher.dispatch<jng::WindowResizeEvent>(JNG_BIND_EVENT_FUNC(SampleLayer::onWindowResize));
    }
private:
    bool onWindowResize(jng::WindowResizeEvent& event)
    {
        m_camera.setProjection(45.f, static_cast<float>(event.getWidth()) / static_cast<float>(event.getHeight()), 01.f, 100.f);
        m_cameraUBO->setData(glm::value_ptr(m_camera.getVP()), sizeof(glm::mat4), 0);
        return false;
    }

    static const jng::VertexLayout LAYOUT;
    jng::Ref<jng::Shader> m_shader;
    jng::Ref<jng::UniformBuffer> m_cameraUBO;
    jng::Ref<jng::UniformBuffer> m_modelUBO;
    jng::Ref<jng::VertexBuffer> m_VBO;
    jng::Ref<jng::IndexBuffer> m_IBO;
    jng::Ref<jng::VertexArray> m_VAO;
    jng::Ref<jng::Texture> m_texture;
    jng::PerspectiveCamera m_camera;
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
    static constexpr unsigned int WindowWidth = 800;
    static constexpr unsigned int WindowHeight = 600;

    App() :
        Engine({
            "Hello Cube!",
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
