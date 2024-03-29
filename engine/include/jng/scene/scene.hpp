/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/GUID.hpp"
#include "jng/scene/camera.hpp"

#include <entt/entt.hpp>

class b2World;

namespace jng {

    class EditorLayer;
    class Entity;
    class Event;
    class SceneSerializer;

    class Scene
    {
    public:
        Scene() = default;
        ~Scene();

        static Ref<Scene> copy(const Ref<Scene>& other);

        Entity createEntity(const std::string& name);
        Entity createEntity(const std::string& name, GUID id);
        Entity duplicateEntity(Entity other);
        void destroyEntity(Entity entity);

        void onCreate(f32 gravity);
        void onDestroy();
        void onUpdate(f32 dt);
        void onEvent(Event& event);

        template<typename Func>
        void each(Func func);
        template<typename Component, typename Func>
        void sort(Func func);
        template<typename Component, typename Func>
        void each(Func func);

        void setViewportSize(f32 width, f32 height) { m_viewportWidth = width; m_viewportHeight = height; }
    private:
        void drawRenderables();
        void drawColliders();

        f32 m_viewportWidth = 0.f;
        f32 m_viewportHeight = 0.f;
        entt::registry m_registry;
        b2World* m_physics2dWorld = nullptr;

        friend class EditorLayer;
        friend class Entity;
        friend class SceneSerializer;
    };

    template<typename Func>
    void Scene::each(Func func)
    {
        m_registry.each([&](entt::entity entityHandle) {
            func({ entityHandle, *this });
        });
    }

    template<typename Component, typename Func>
    void Scene::sort(Func func)
    {
        m_registry.sort<Component>(func);
    }

    template<typename Component, typename Func>
    void Scene::each(Func func)
    {
        auto view = m_registry.view<Component>();
        for (auto entity : view)
            func({ entity, *this });
    }

} // namespace jng
