//
// Copyright (C) 2022-2023 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in vec4 v_Color;
layout(location = 2) flat in uint v_TexIndex;
layout(location = 3) flat in int v_EntityID;

layout(location = 0) out vec4 o_fragColor;
layout(location = 1) out int o_entityID;

layout(binding = 0) uniform sampler2D u_Textures[16];

void main()
{
    o_fragColor = v_Color;
    
    switch (v_TexIndex)
    {
    case  0: o_fragColor *= texture(u_Textures[0], v_TexCoord); break;
    case  1: o_fragColor *= texture(u_Textures[1], v_TexCoord); break;
    case  2: o_fragColor *= texture(u_Textures[2], v_TexCoord); break;
    case  3: o_fragColor *= texture(u_Textures[3], v_TexCoord); break;
    case  4: o_fragColor *= texture(u_Textures[4], v_TexCoord); break;
    case  5: o_fragColor *= texture(u_Textures[5], v_TexCoord); break;
    case  6: o_fragColor *= texture(u_Textures[6], v_TexCoord); break;
    case  7: o_fragColor *= texture(u_Textures[7], v_TexCoord); break;
    case  8: o_fragColor *= texture(u_Textures[8], v_TexCoord); break;
    case  9: o_fragColor *= texture(u_Textures[9], v_TexCoord); break;
    case 10: o_fragColor *= texture(u_Textures[10], v_TexCoord); break;
    case 11: o_fragColor *= texture(u_Textures[11], v_TexCoord); break;
    case 12: o_fragColor *= texture(u_Textures[12], v_TexCoord); break;
    case 13: o_fragColor *= texture(u_Textures[13], v_TexCoord); break;
    case 14: o_fragColor *= texture(u_Textures[14], v_TexCoord); break;
    case 15: o_fragColor *= texture(u_Textures[15], v_TexCoord); break;
    }

    if (o_fragColor.a == 0.0)
        discard;

    o_entityID = v_EntityID;
}
