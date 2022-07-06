//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec3 v_Color;
layout(location = 1) in vec2 v_TexCoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D u_Texture;

void main()
{
    fragColor = vec4(v_Color, 1.0) * texture(u_Texture, v_TexCoord);
}
