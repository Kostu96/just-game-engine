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
        template<typename T, typename ...Args>
        T& addComponent(Args&& ...args);

        template<typename T>
        bool hasComponent();

        template<typename T>
        T& getComponent();

        bool operator==(const Entity& other) const { return m_handle == other.m_handle; }
        bool operator!=(const Entity& other) const { return m_handle != other.m_handle; }
    private:
        Entity(entt::entity handle, Scene& scene);
        
        entt::entity m_handle;
        Scene& m_sceneRef;

        friend class Scene;
    };

    template<typename T, typename ...Args>
    T& Entity::addComponent(Args&& ...args)
    {
        return m_sceneRef.m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
    }

    template<typename T>
    bool Entity::hasComponent()
    {
        return m_sceneRef.m_registry.all_of<T>(m_handle);
    }

    template<typename T>
    T& Entity::getComponent()
    {
        return m_sceneRef.m_registry.get<T>(m_handle);
    }

} // namespace jng
