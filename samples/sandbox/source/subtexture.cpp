/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "subtexture.hpp"

SubTexture::SubTexture(const jng::Ref<jng::Texture>& texture, const glm::vec2 bottomLeft, const glm::vec2 topRight) :
    m_texture{ texture },
    m_texCoords{
        { bottomLeft.x / static_cast<float>(texture->getProperties().width), bottomLeft.y / static_cast<float>(texture->getProperties().height)},
        { topRight.x / static_cast<float>(texture->getProperties().width), bottomLeft.y / static_cast<float>(texture->getProperties().height) },
        { topRight.x / static_cast<float>(texture->getProperties().width), topRight.y / static_cast<float>(texture->getProperties().height) },
        { bottomLeft.x / static_cast<float>(texture->getProperties().width), topRight.y / static_cast<float>(texture->getProperties().height) }
    } {}
