/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scene/entity.hpp"

#include "jng/scene/components.hpp"

namespace jng {

    Entity::Entity(entt::entity handle, Scene& scene) :
        m_handle{ handle },
        m_sceneRef{ &scene }
    {}

    GUID Entity::getGUID() const
    {
        return getComponent<IDComponent>().ID;
    }

    const std::string& Entity::getTag() const
    {
        return getComponent<TagComponent>().Tag;
    }

} // namespace jng
