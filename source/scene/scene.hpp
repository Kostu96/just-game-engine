/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <entt/entt.hpp>

namespace k2d {

    class Entity;

    class Scene
    {
    public:
        Scene() = default;

        Entity createEntity();
        void render();
    private:
        entt::registry m_registry;

        friend class Entity;
    };

} // namespace k2d
