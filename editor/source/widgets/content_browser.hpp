/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/renderer/texture.hpp>

namespace jng {

    struct EditorContext;

    class ContentBrowserWindow
    {
    public:
        ContentBrowserWindow(EditorContext& context);

        void onImGuiUpdate();
    private:
        EditorContext& m_context;
        Ref<Texture> m_fileIcon;
        Ref<Texture> m_directoryIcon;
    };

} // namespace jng
