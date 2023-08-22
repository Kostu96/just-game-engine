/*
 * Copyright (C) 2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"

namespace jng {

    struct FontData;

    class Font
    {
    public:
        explicit Font(const std::filesystem::path& filepath);

        const FontData* getFontData() const { return m_data.get(); }
    private:
        Scope<FontData> m_data;
    };

} // namespace jng
