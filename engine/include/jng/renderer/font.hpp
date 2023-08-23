/*
 * Copyright (C) 2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"

namespace jng {

    class Texture;
    struct FontData;

    class Font
    {
    public:
        explicit Font(const std::filesystem::path& filepath);

        const FontData* getFontData() const { return m_data.get(); }
        Ref<Texture> getAtlasTexture() const { return m_atlasTexture; }

        static Ref<Font> getDefaultFont();
    private:
        Scope<FontData> m_data;
        Ref<Texture> m_atlasTexture;
    };

} // namespace jng
