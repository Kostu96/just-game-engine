/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scene/entity.hpp"

namespace jng {

    Entity::Entity(entt::entity handle, Scene& scene) :
        m_handle{ handle },
        m_sceneRef{ &scene }
    {}

} // namespace jng
