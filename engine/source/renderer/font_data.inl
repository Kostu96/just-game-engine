/*
 * Copyright (C) 2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

namespace jng {

    struct FontData
    {
        std::vector<msdf_atlas::GlyphGeometry> glyphs;
        msdf_atlas::FontGeometry geometry;
    };

} // namespace jng
