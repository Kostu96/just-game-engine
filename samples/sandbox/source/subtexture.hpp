/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/jng.hpp>

class SubTexture
{
public:
    SubTexture(const jng::Ref<jng::Texture>& texture, const glm::vec2 bottomLeft, const glm::vec2 topRight);

    const jng::Ref<jng::Texture>& getTexture() const { return m_texture; }
    const glm::vec2* getTextureCoords() const { return m_texCoords; }
private:
    const jng::Ref<jng::Texture>& m_texture;
    glm::vec2 m_texCoords[4];
};
