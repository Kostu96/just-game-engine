/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define JNG_DECLARE_MAIN
#include <jng/jng.hpp>

const char* vert_shader_ogl = R"(
#version 450

layout(location = 0) in vec2 a_Position;

out vec2 v_Color;

void main()
{
    v_Color = vec2(a_Position.x + 0.5, a_Position.y + 0.5);
    gl_Position = vec4(a_Position, 0.0, 1.0);
}
)";

const char* frag_shader_ogl = R"(
#version 450

in vec2 v_Color;

out vec4 fragColor;

void main()
{
    fragColor = vec4(v_Color, 0.0, 1.0);
}
)";

const char* vert_shader_d3d = R"(
struct VSOut
{
    float2 color : v_Color;
    float4 position : SV_Position;
};

VSOut main(float2 position : a_Position)
{
    VSOut vso;
    vso.color = float2(position.x + 0.5f, position.y + 0.5f);
    vso.position = float4(position, 0.0f, 1.0f);
    return vso;
}
)";

const char* frag_shader_d3d = R"(
float4 main(float2 color : v_Color) : SV_Target
{
    return float4(color, 0.0f, 1.0f);
}
)";

const glm::vec2 vertices[]{
    { -0.5f, -0.5f },
    {  0.0f,  0.5f },
    {  0.5f, -0.5f }
};

class SampleLayer :
    public jng::Layer
{
public:
    SampleLayer() :
        m_shader{ jng::RendererAPI::getRendererBackend() == jng::RendererBackend::Direct3D ?
            jng::Shader::create(vert_shader_d3d, frag_shader_d3d) :
            jng::Shader::create(vert_shader_ogl, frag_shader_ogl) },
        m_VBO{ jng::VertexBuffer::create(vertices, sizeof(vertices)) },
        m_VAO{ jng::VertexArray::create(m_VBO, LAYOUT, m_shader) }
    {
        // NOTE: These can be bound once at the begining because they're only one used.
        m_shader->bind();
        m_VAO->bind();
    }

    void onUpdate(float /*dt*/) override
    {
        jng::RendererAPI::clear({ 0.1f, 0.1f, 0.2f });

        jng::RendererAPI::draw(3);
    }
private:
    static const jng::VertexLayout LAYOUT;
    jng::Ref<jng::Shader> m_shader;
    jng::Ref<jng::VertexBuffer> m_VBO;
    jng::Ref<jng::IndexBuffer> m_IBO;
    jng::Ref<jng::VertexArray> m_VAO;
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
