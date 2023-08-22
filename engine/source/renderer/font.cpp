/*
 * Copyright (C) 2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/font.hpp"

#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <msdfgen.h>

namespace jng {

    static msdfgen::FreetypeHandle* freetypeHandle = nullptr;

    Font::Font(const std::filesystem::path& filepath)
    {
        std::string strpath = filepath.string();
        msdfgen::FontHandle* font = msdfgen::loadFont(freetypeHandle, strpath.c_str());
        JNG_CORE_ASSERT(font, std::string{ "Failed to load font: " } + strpath);

        msdfgen::Shape shape;
        if (msdfgen::loadGlyph(shape, font, 'A')) {
            shape.normalize();
            msdfgen::edgeColoringSimple(shape, 3.0);
            msdfgen::Bitmap<float, 3> msdf(32, 32);
            msdfgen::generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));
        }

        msdfgen::destroyFont(font);
    }

    static struct FreetypeInitializer
    {
        FreetypeInitializer()
        {
            freetypeHandle = msdfgen::initializeFreetype();
            JNG_CORE_ASSERT(freetypeHandle, "Could not initialize Freetype");
        }

        ~FreetypeInitializer()
        {
            msdfgen::deinitializeFreetype(freetypeHandle);
        }
    }
    globalFreetypeInitializer;

} // namespace jng
