//
// Copyright (C) 2023 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in vec4 v_Color;
layout(location = 2) flat in int v_EntityID;

layout(location = 0) out vec4 o_fragColor;
layout(location = 1) out int o_entityID;

layout(binding = 0) uniform sampler2D u_FontAtlas;

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

float screenPixelRange()
{
    vec2 unitRange = vec2(pixelxRange) / vec2(textureSize(msdf, 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(texCoord);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main()
{
    o_fragColor = v_Color * texture(u_FontAtlas, v_TexCoord);

    if (o_fragColor.a == 0.0)
        discard;

    o_entityID = v_EntityID;
}
