/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/GUID.hpp"
#include "jng/scene/camera.hpp"

#include <entt/entt.hpp>
#include <string>

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

        void onCreate(float gravity);
        void onDestroy();
        void onUpdate(float dt);
        void onEvent(Event& event);

        template<typename Func>
        void each(Func func);

        void setViewportSize(float width, float height);
    private:
        void drawRenderables();
        void drawColliders();

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

} // namespace jng
