//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec4 v_Color;

layout(location = 0) out vec4 o_fragColor;

void main()
{
    o_fragColor = v_Color;
}
