/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/scene/scene.hpp"

#include <entt/entt.hpp>

namespace jng {

    class Scene;

    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene& scene);

        template<typename Component, typename ...Args>
        Component& addComponent(Args&& ...args);
        template<typename Component, typename ...Args>
        Component& addOrReplaceComponent(Args&& ...args);
        template<typename Component>
        void removeComponent();
        template<typename Component>
        bool hasComponent() const;
        template<typename Component>
        Component& getComponent();
        template<typename Component>
        const Component& getComponent() const;
        template<typename Component, typename ...Args>
        Component& getOrAddComponent(Args&& ...args);

        GUID getGUID() const;
        const std::string& getTag() const;

        Scene* getScene() { return m_sceneRef; }

        bool operator==(const Entity& other) const { return m_handle == other.m_handle; }
        bool operator!=(const Entity& other) const { return m_handle != other.m_handle; }
        operator bool() { return m_handle != entt::null; }
        operator void*() { return reinterpret_cast<void*>(m_handle); }
    private:
        entt::entity m_handle{ entt::null };
        Scene* m_sceneRef{ nullptr };

        friend class Scene;
    };

    template<typename Component, typename ...Args>
    Component& Entity::addComponent(Args&& ...args)
    {
        JNG_CORE_ASSERT(!hasComponent<Component>(), "Entity can't have more that one component of the same type!");
        return m_sceneRef->m_registry.emplace<Component>(m_handle, std::forward<Args>(args)...);
    }

    template<typename Component, typename ...Args>
    inline Component& Entity::addOrReplaceComponent(Args && ...args)
    {
        return m_sceneRef->m_registry.emplace_or_replace<Component>(m_handle, std::forward<Args>(args)...);
    }

    template<typename Component>
    void Entity::removeComponent()
    {
        m_sceneRef->m_registry.remove<Component>(m_handle);
    }

    template<typename Component>
    bool Entity::hasComponent() const
    {
        return m_sceneRef->m_registry.all_of<Component>(m_handle);
    }

    template<typename Component>
    Component& Entity::getComponent()
    {
        JNG_CORE_ASSERT(hasComponent<Component>(), "Entity doesn't have component of that type!");
        return m_sceneRef->m_registry.get<Component>(m_handle);
    }

    template<typename Component>
    const Component& Entity::getComponent() const
    {
        JNG_CORE_ASSERT(hasComponent<Component>(), "Entity doesn't have component of that type!");
        return m_sceneRef->m_registry.get<Component>(m_handle);
    }

    template<typename Component, typename ...Args>
    inline Component& Entity::getOrAddComponent(Args&& ...args)
    {
        return m_sceneRef->m_registry.get_or_emplace<Component>(m_handle, std::forward<Args>(args)...);
    }

} // namespace jng
