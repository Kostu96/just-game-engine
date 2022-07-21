/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/scene/camera.hpp"

#include <entt/entt.hpp>
#include <string>

namespace jng {

    class EditorLayer;
    class Entity;
    class Event;
    class SceneSerializer;

    class Scene
    {
    public:
        Scene() = default;

        Entity createEntity(const std::string& name);
        void destroyEntity(Entity entity);

        
        void onCreate();
        void onDestroy();
        void onUpdate(float dt);
        void onEvent(Event& event);

        template<typename Func>
        void each(Func func);

        //Camera* getActiveCamera();
        void setViewportSize(float width, float height);
    private:
        void drawSprites();

        entt::registry m_registry;
        //Camera* m_camera = nullptr;

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
