/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/shader_sources.hpp"

namespace jng {
    namespace shader_source {

        const char* basic_vert = R"(
#version 450

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_VP;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;

void main()
{
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;

    gl_Position = u_VP * vec4(a_Position, 0.0, 1.0);
}

)";

        const char* basic_frag = R"(
#version 450

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

out vec4 fragColor;

void main()
{
    fragColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
}

)";

    } // namespace shader_source
} // namespace jng
