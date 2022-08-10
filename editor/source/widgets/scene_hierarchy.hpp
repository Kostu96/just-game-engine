/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/scene/entity.hpp>

namespace jng {

    struct EditorContext;

    class SceneHierarchyWindow
    {
    public:
        SceneHierarchyWindow(EditorContext& context) : m_context{ context } {}

        void onImGuiUpdate();
    private:
        void updateSceneHierarchyItem(Entity entity);
        
        EditorContext& m_context;
    };

} // namespace jng
