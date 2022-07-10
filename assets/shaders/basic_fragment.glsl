//
// Copyright (C) 2022 Konstanty Misiak
//
// SPDX-License-Identifier: MIT
//

#version 450 core

layout(location = 0) in vec2 v_TexCoord;
layout(location = 1) in vec4 v_Color;
layout(location = 2) in float v_TexIndex;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D u_Textures[16];

void main()
{
    fragColor = v_Color;
    
    switch (int(v_TexIndex))
    {
    case  0: fragColor *= texture(u_Textures[0], v_TexCoord); break;
    case  1: fragColor *= texture(u_Textures[1], v_TexCoord); break;
    case  2: fragColor *= texture(u_Textures[2], v_TexCoord); break;
    case  3: fragColor *= texture(u_Textures[3], v_TexCoord); break;
    case  4: fragColor *= texture(u_Textures[4], v_TexCoord); break;
    case  5: fragColor *= texture(u_Textures[5], v_TexCoord); break;
    case  6: fragColor *= texture(u_Textures[6], v_TexCoord); break;
    case  7: fragColor *= texture(u_Textures[7], v_TexCoord); break;
    case  8: fragColor *= texture(u_Textures[8], v_TexCoord); break;
    case  9: fragColor *= texture(u_Textures[9], v_TexCoord); break;
    case 10: fragColor *= texture(u_Textures[10], v_TexCoord); break;
    case 11: fragColor *= texture(u_Textures[11], v_TexCoord); break;
    case 12: fragColor *= texture(u_Textures[12], v_TexCoord); break;
    case 13: fragColor *= texture(u_Textures[13], v_TexCoord); break;
    case 14: fragColor *= texture(u_Textures[14], v_TexCoord); break;
    case 15: fragColor *= texture(u_Textures[15], v_TexCoord); break;
    }
}
