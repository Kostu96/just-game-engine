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

    const std::string& Entity::getTag()
    {
        return getComponent<TagComponent>().Tag;
    }

    bool Entity::hasParent()
    {
        return hasComponent<ParentComponent>();
    }

    bool Entity::hasChildren()
    {
        return hasComponent<ChildrenComponent>();
    }

    GUID Entity::getGUID()
    {
        return getComponent<IDComponent>().ID;
    }

} // namespace jng
