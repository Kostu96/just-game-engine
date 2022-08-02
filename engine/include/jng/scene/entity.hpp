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

        template<typename Component, typename ...Args>
        Component& addComponent(Args&& ...args);

        template<typename Component>
        void removeComponent();

        template<typename Component>
        bool hasComponent();

        template<typename Component>
        Component& getComponent();

        Scene* getScene() { return m_sceneRef; }

        bool operator==(const Entity& other) const { return m_handle == other.m_handle; }
        bool operator!=(const Entity& other) const { return m_handle != other.m_handle; }
        operator bool() { return m_handle != entt::null; }
        operator void*() { return reinterpret_cast<void*>(m_handle); }
    private:
        Entity(entt::entity handle, Scene& scene);
        
        entt::entity m_handle{ entt::null };
        Scene* m_sceneRef{ nullptr };

        friend class EditorLayer;
        friend class Scene;
    };

    template<typename Component, typename ...Args>
    Component& Entity::addComponent(Args&& ...args)
    {
        JNG_CORE_ASSERT(!hasComponent<Component>(), "Entity can't have more that one component of the same type!");
        return m_sceneRef->m_registry.emplace<Component>(m_handle, std::forward<Args>(args)...);
    }

    template<typename Component>
    void Entity::removeComponent()
    {
        m_sceneRef->m_registry.remove<Component>(m_handle);
    }

    template<typename Component>
    bool Entity::hasComponent()
    {
        return m_sceneRef->m_registry.all_of<Component>(m_handle);
    }

    template<typename Component>
    Component& Entity::getComponent()
    {
        return m_sceneRef->m_registry.get<Component>(m_handle);
    }

} // namespace jng
