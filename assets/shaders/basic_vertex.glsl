//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in uint a_TexIndex;

layout(location = 0) out vec2 v_TexCoord;
layout(location = 1) out vec4 v_Color;
layout(location = 2) out uint v_TexIndex;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_VP;
};

void main()
{
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;
    gl_Position = u_VP * vec4(a_Position, 0.0, 1.0);
}
