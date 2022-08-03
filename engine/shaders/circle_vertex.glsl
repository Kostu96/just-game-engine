//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_LocalPosition;
layout(location = 2) in float a_Thickness;
layout(location = 3) in float a_Fade;
layout(location = 4) in vec4 a_Color;
layout(location = 5) in int a_EntityID;

layout(location = 0) out vec2 v_LocalPosition;
layout(location = 1) out float v_Thickness;
layout(location = 2) out float v_Fade;
layout(location = 3) out vec4 v_Color;
layout(location = 4) out int v_EntityID;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_VP;
};

void main()
{
    v_LocalPosition = a_LocalPosition;
    v_Thickness = a_Thickness;
    v_Fade = a_Fade;
    v_Color = a_Color;
    v_EntityID = a_EntityID;

    gl_Position = u_VP * vec4(a_Position, 1.0);
}
