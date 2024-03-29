//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

layout(location = 0) out vec4 v_Color;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_VP;
};

void main()
{
    v_Color = a_Color;

    gl_Position = u_VP * vec4(a_Position, 1.0);
}
