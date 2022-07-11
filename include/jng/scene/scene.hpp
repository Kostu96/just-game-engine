/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <entt/entt.hpp>

namespace jng {

    class Entity;

    class Scene
    {
    public:
        Scene() = default;

        Entity createEntity();

        void onUpdate();

        template<typename Func>
        void each(Func func);
    private:
        entt::registry m_registry;

        friend class Entity;
    };

    template<typename Func>
    void Scene::each(Func func)
    {
        m_registry.each([&](entt::entity entityHandle) {
            func({ entityHandle, *this });
        });
    }

} // namespace jng
