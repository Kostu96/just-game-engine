//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec2 v_Color;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(v_Color, 0.0, 1.0);
}
