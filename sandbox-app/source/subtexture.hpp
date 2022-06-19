/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <k2d/k2d.hpp>

class SubTexture
{
public:
    SubTexture(const k2d::Ref<k2d::Texture>& texture, const glm::vec2 bottomLeft, const glm::vec2 topRight);

    const k2d::Ref<k2d::Texture>& getTexture() const { return m_texture; }
    const glm::vec2* getTextureCoords() const { return m_texCoords; }
private:
    const k2d::Ref<k2d::Texture>& m_texture;
    glm::vec2 m_texCoords[4];
};
