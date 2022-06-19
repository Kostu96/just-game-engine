/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/shader_sources.hpp"

#if defined(K2D_RENDER_API_DIRECT3D)

namespace k2d {
    namespace shader_source {

        const char* basic_vert = R"(
struct VSOut
{
    float2 texCoord : v_TexCoord;
    float4 color : v_Color;
    float texIndex : v_TexIndex;
    float4 position : SV_Position;
};

VSOut main(float2 position : a_Position, float2 texCoord : a_TexCoord, float4 color : a_Color, float texIndex : a_TexIndex)
{
    VSOut vso;
    vso.texCoord = texCoord;
    vso.color = color;
    vso.texIndex = texIndex;
    vso.position = float4(position, 0.0, 1.0);
    return vso;
}

//uniform mat4 u_VP;
//void main()
//{
//    gl_Position = u_VP * vec4(a_Position, 0.0, 1.0);
//}

)";

        const char* basic_frag = R"(
float4 main(float2 texCoord : v_TexCoord, float4 color : v_Color, float texIndex : v_TexIndex) : SV_Target
{
    return color;
}

//uniform sampler2D u_Textures[32];
//void main()
//{
//    fragColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
//}

)";

    } // namespace shader_source
} // namespace k2d

#endif
