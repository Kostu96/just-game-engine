//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in vec4 v_Color;
layout(location = 2) in float v_TexIndex;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main()
{
    fragColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
}
