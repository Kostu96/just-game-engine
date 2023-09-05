//
// Copyright (C) 2023 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in int a_EntityID;

layout(location = 0) out vec2 v_TexCoord;
layout(location = 1) out vec4 v_Color;
layout(location = 2) out int v_EntityID;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_VP;
};

void main()
{
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    v_EntityID = a_EntityID;

    gl_Position = u_VP * vec4(a_Position, 1.0);
}
