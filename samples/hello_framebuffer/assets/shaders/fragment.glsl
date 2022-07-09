//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec2 v_TexCoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D u_Texture;

void main()
{
    fragColor = texture(u_Texture, v_TexCoord);
}
