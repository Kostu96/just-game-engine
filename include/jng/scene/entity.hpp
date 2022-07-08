/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <entt/entt.hpp>

namespace jng {

    class Scene;

    class Entity
    {
    public:
    private:
        Entity(entt::entity handle, Scene& scene);
        
        entt::entity m_handle;
        Scene& m_sceneRef;

        friend class Scene;
    };

} // namespace jng
