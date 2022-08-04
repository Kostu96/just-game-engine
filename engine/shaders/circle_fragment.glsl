//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec2 v_LocalPosition;
layout(location = 1) flat in float v_Thickness;
layout(location = 2) flat in float v_Fade;
layout(location = 3) in vec4 v_Color;
layout(location = 4) flat in int v_EntityID;

layout(location = 0) out vec4 o_fragColor;
layout(location = 1) out int o_entityID;

void main()
{
    float radius = 1.0 - length(v_LocalPosition);
    float alpha = smoothstep(0.0, v_Fade, radius);
    alpha *= smoothstep(v_Thickness + v_Fade, v_Thickness, radius);

    if (alpha == 0.0)
        discard;

    o_fragColor = v_Color;
    o_fragColor.a *= alpha;

    o_entityID = v_EntityID;
}
