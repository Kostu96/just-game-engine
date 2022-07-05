//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec2 a_Position;

layout(location = 0) out vec2 v_Color;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_VP;
};

void main()
{
    v_Color = vec2(a_Position.x + 0.5, a_Position.y + 0.5);
    gl_Position = u_VP * vec4(a_Position, 0.0, 1.0);
}
