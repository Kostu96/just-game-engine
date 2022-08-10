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

    bool Entity::hasParent() const
    {
        return hasComponent<ParentComponent>();
    }

    bool Entity::hasChildren() const
    {
        return hasComponent<ChildrenComponent>();
    }

    void Entity::setParent(Entity newParent)
    {
        addOrReplaceComponent<LocalTransformComponent>().isDirty = true;

        auto& selfParentComponent = getOrAddComponent<ParentComponent>();

        if (selfParentComponent.parent)
        {
            auto& selfParentChildrenComponent = selfParentComponent.parent.getComponent<ChildrenComponent>();
            selfParentChildrenComponent.children.remove(*this);
            if (selfParentChildrenComponent.children.empty())
                selfParentComponent.parent.removeComponent<ChildrenComponent>();
        }

        selfParentComponent.parent = newParent;

        auto& newParentChildren = newParent.getOrAddComponent<ChildrenComponent>();
        newParentChildren.children.emplace_back(*this);
    }

    void Entity::removeParent()
    {
        if (hasParent())
        {
            auto selfParentComponent = getComponent<ParentComponent>();
            auto& selfParentChildrenComponent = selfParentComponent.parent.getComponent<ChildrenComponent>();
            selfParentChildrenComponent.children.remove(*this);
            if (selfParentChildrenComponent.children.empty())
                selfParentComponent.parent.removeComponent<ChildrenComponent>();

            removeComponent<ParentComponent>();
            removeComponent<LocalTransformComponent>();
        }
    }

} // namespace jng
