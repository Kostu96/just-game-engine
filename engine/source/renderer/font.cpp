/*
 * Copyright (C) 2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/font.hpp"
#include "renderer/texture.hpp"
#include "core/engine.hpp"

#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <msdfgen.h>

#include "renderer/font_data.inl"

namespace jng {

    static msdfgen::FreetypeHandle* freetypeHandle = nullptr;

    template <typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
    static Ref<Texture> createAndCacheAtlas(const std::vector<msdf_atlas::GlyphGeometry>& glyphs, u32 width, u32 height)
    {
        msdf_atlas::GeneratorAttributes attribs;
        attribs.config.overlapSupport = true;
        attribs.scanlinePass = true;

        msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator{ (int)width, (int)height };
        generator.setAttributes(attribs);
        generator.setThreadCount(4); // TODO: hwInfo
        generator.generate(glyphs.data(), (int)glyphs.size());

        msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

        Texture::Properties props{
            TextureFormat::RGB8,
            (u32)bitmap.width, (u32)bitmap.height
        };
        Ref<Texture> texture = makeRef<Texture>(props);
        texture->setData((void *)bitmap.pixels, (size_t)bitmap.width * (size_t)bitmap.height);

        return texture;
    }

    Font::Font(const std::filesystem::path& filepath) :
        m_data{ makeScope<FontData>() }
    {
        std::string strpath = filepath.string();
        msdfgen::FontHandle* font = msdfgen::loadFont(freetypeHandle, strpath.c_str());
        JNG_CORE_ASSERT(font, std::string{ "Failed to load font: " } + strpath);

        static const struct { u32 begin, end; } charsetRanges[] = {
            { 0x0020, 0x00FF } // Basic Latin + Latin Supplement
        };

        msdf_atlas::Charset charset;
        for (auto& range : charsetRanges)
            for (u32 c = range.begin; c <= range.end; c++)
                charset.add(c);

        m_data->geometry = msdf_atlas::FontGeometry{ &m_data->glyphs };
        int glyphsLoaded = m_data->geometry.loadCharset(font, 1.0, charset);
        JNG_CORE_TRACE("Loaded {} glyphs from font (out of {}).", glyphsLoaded, charset.size());

        double emSize = 40.0;
        msdf_atlas::TightAtlasPacker packer;
        packer.setPixelRange(2.0);
        packer.setMiterLimit(1.0);
        packer.setPadding(0);
        packer.setScale(emSize);
        int remaining = packer.pack(m_data->glyphs.data(), (int)m_data->glyphs.size());
        JNG_CORE_ASSERT(remaining == 0, "Atlas packer could not pack all glyphs");

        int width, height;
        packer.getDimensions(width, height);
        emSize = packer.getScale();

        // if MSDF || MTSDF
        static constexpr u64 LCG_MULTIPLIER = 6364136223846793005ull;
        static constexpr u64 LCG_INCREMENT = 1442695040888963407ull;
        static constexpr f64 DEFAULT_ANGLE_THRESHOLD = 3.0;
        u64 glyphSeed = 1;
        for (auto& glyph : m_data->glyphs)
        {
            glyphSeed *= LCG_MULTIPLIER;
            glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
        }

        m_atlasTexture = createAndCacheAtlas<u8, f32, 3, msdf_atlas::msdfGenerator>(m_data->glyphs, width, height);

        msdfgen::destroyFont(font);
    }

    Ref<Font> Font::getDefaultFont()
    {
        static Ref<Font> defaultFont;

        if (!defaultFont) {
            std::filesystem::path assetsDir = Engine::get().getProperties().assetsDirectory;
            defaultFont = makeRef<Font>(assetsDir / "fonts/OpenSans-Regular.ttf");
        }

        return defaultFont;
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
