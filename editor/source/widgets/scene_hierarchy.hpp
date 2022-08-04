/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

namespace jng {

    struct EditorContext;

    class SceneHierarchyWindow
    {
    public:
        SceneHierarchyWindow(EditorContext& context) : m_context{ context } {}

        void onImGuiUpdate();
    private:
        EditorContext& m_context;
    };

} // namespace jng
