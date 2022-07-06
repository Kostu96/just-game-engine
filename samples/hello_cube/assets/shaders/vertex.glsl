//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

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
