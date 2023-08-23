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
    const float pixelRange = 2.0; // this is set to the same value for msdfgen
    vec2 unitRange = vec2(pixelRange) / vec2(textureSize(u_FontAtlas, 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(v_TexCoord);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main()
{
    vec3 msd = texture(u_FontAtlas, v_TexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPixelDistance = screenPixelRange() * (sd - 0.5);
    float opacity = clamp(screenPixelDistance + 0.5, 0.0, 1.0);

    if (opacity == 0.0)
        discard;

    o_fragColor = vec4(v_Color.rgb, opacity * v_Color.a);
    o_entityID = v_EntityID;
}
