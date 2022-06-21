/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define JNG_DECLARE_MAIN
#include <jng/jng.hpp>

const char* vert_shader = R"(
#version 450

layout(location = 0) in vec2 a_Position;

out vec2 v_Color;

void main()
{
    v_Color = vec2(a_Position.x + 0.5, a_Position.y + 0.5);
    gl_Position = vec4(a_Position, 0.0, 1.0);
}

)";

const char* frag_shader = R"(
#version 450

in vec2 v_Color;

out vec4 fragColor;

void main()
{
    fragColor = vec4(v_Color, 0.0, 1.0);
}

)";

const glm::vec2 vertices[]{
    { -0.5f, -0.5f },
    {  0.5f, -0.5f },
    {  0.0f,  0.5f }
};

const jng::uint32 indices[]{ 0, 1, 2 };

class SampleLayer :
    public jng::Layer
{
public:
    SampleLayer() :
        m_shader{ jng::Shader::create(vert_shader, frag_shader) },
        m_VBO{ jng::VertexBuffer::create(vertices, sizeof(vertices)) },
        m_IBO{ jng::IndexBuffer::create(indices, sizeof(indices)) },
        m_VAO{ jng::VertexArray::create(m_VBO, {{ jng::LayoutElement::DataType::Float2, "a_Position" }}, m_shader) }
    {
        m_VAO->setIndexBuffer(m_IBO);

        // NOTE: These can be bound once at the begining because they're only one used.
        m_shader->bind();
        m_VAO->bind();
    }

    void onUpdate(float /*dt*/) override
    {
        jng::RendererAPI::clear({ 0.1f, 0.1f, 0.2f });

        jng::RendererAPI::drawIndexed(m_VAO);
    }
private:
    jng::Ref<jng::Shader> m_shader;
    jng::Ref<jng::VertexBuffer> m_VBO;
    jng::Ref<jng::IndexBuffer> m_IBO;
    jng::Ref<jng::VertexArray> m_VAO;
};

class App :
    public jng::Engine
{
public:
    static constexpr unsigned int WindowWidth = 800;
    static constexpr unsigned int WindowHeight = 600;

    App() :
        Engine("Hello Triangle!", WindowWidth, WindowHeight)
    {
        getLayerStack().pushLayer(new SampleLayer{});
    }
};

jng::Engine* createApp() {
    return new App{};
}
