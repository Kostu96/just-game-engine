/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <entt/entt.hpp>

namespace k2d {

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

} // namespace k2d
