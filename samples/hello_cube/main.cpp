/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define JNG_DECLARE_MAIN
#include <jng/jng.hpp>

const char* vert_shader_ogl = R"(
#version 450

layout(location = 0) in vec3 a_Position;

out vec3 v_Color;

uniform mat4 u_VP;

void main()
{
    v_Color = vec3(a_Position.x + 0.5, a_Position.y + 0.5, a_Position.z + 0.5);
    gl_Position = u_VP * vec4(a_Position, 1.0);
}
)";

const char* frag_shader_ogl = R"(
#version 450

in vec3 v_Color;

out vec4 fragColor;

void main()
{
    fragColor = vec4(v_Color, 1.0);
}
)";

const char* vert_shader_d3d = R"(
struct VSOut
{
    float3 color : v_Color;
    float4 position : SV_Position;
};

VSOut main(float3 position : a_Position)
{
    VSOut vso;
    vso.color = float3(position.x + 0.5f, position.y + 0.5f, position.z + 0.5f);
    vso.position = float4(position, 1.0f);
    return vso;
}
)";

const char* frag_shader_d3d = R"(
float4 main(float3 color : v_Color) : SV_Target
{
    return float4(color, 1.0f);
}
)";

const glm::vec3 vertices[]{
    { -0.5, -0.5,  0.5 },
    {  0.5, -0.5,  0.5 },
    {  0.5,  0.5,  0.5 },
    { -0.5,  0.5,  0.5 },

    { -0.5, -0.5, -0.5 },
    {  0.5, -0.5, -0.5 },
    {  0.5,  0.5, -0.5 },
    { -0.5,  0.5, -0.5 }
};

const jng::uint32 indices[]{
    // front
    0, 3, 2,
    2, 1, 0,
    // right
    1, 2, 6,
    6, 5, 1,
    // back
    7, 4, 5,
    5, 6, 7,
    // left
    4, 7, 3,
    3, 0, 4,
    // bottom
    4, 0, 1,
    1, 5, 4,
    // top
    3, 7, 6,
    6, 2, 3
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
        m_IBO{ jng::IndexBuffer::create(indices, sizeof(indices)) },
        m_VAO{ jng::VertexArray::create(m_VBO, LAYOUT, m_shader) }
    {
        m_VAO->setIndexBuffer(m_IBO);

        // NOTE: These can be bound once at the begining because they're only one used.
        m_shader->bind();
        m_VAO->bind();

        glm::mat4 projection = glm::ortho(-2.f, 2.f, -1.5f, 1.5f);
        glm::mat4 vp = projection * glm::mat4{1.f}; // View

        m_shader->set("u_VP", vp);
    }

    void onUpdate(float /*dt*/) override
    {
        jng::RendererAPI::clear({ 0.1f, 0.1f, 0.2f });

        jng::RendererAPI::drawIndexed(m_VAO);
    }
private:
    static const jng::VertexLayout LAYOUT;
    jng::Ref<jng::Shader> m_shader;
    jng::Ref<jng::VertexBuffer> m_VBO;
    jng::Ref<jng::IndexBuffer> m_IBO;
    jng::Ref<jng::VertexArray> m_VAO;
};

const jng::VertexLayout SampleLayer::LAYOUT{ { jng::LayoutElement::DataType::Float3, "a_Position" } };

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
