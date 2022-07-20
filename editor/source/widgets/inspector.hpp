/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/renderer/texture.hpp>

namespace jng {

    struct EditorContext;

    class InspectorWindow
    {
    public:
        InspectorWindow(EditorContext& context);

        void onImGuiUpdate();
    private:
        EditorContext& m_context;
        Ref<Texture> m_checkerboard;
    };

} // namespace jng
