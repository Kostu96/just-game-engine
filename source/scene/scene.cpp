/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scene/scene.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer2d.hpp"
#include "scene/components.hpp"
#include "scene/entity.hpp"
#include "scripting/native_script.hpp"

#include <glm/gtx/matrix_decompose.hpp>

namespace jng {

    Entity Scene::createEntity(const std::string& name)
    {
        auto entity = m_registry.create();
        m_registry.emplace<TagComponent>(entity, name);
        m_registry.emplace<TransformComponent>(entity);

        return Entity{ entity, *this };
    }

    void Scene::destroyEntity(Entity entity)
    {
        m_registry.destroy(entity.m_handle);
    }

    Camera* Scene::getActiveCamera()
    {
        if (!m_camera)
        {
            auto view = m_registry.view<CameraComponent>();
            if (view.size() > 0)
                m_camera = &view.get<CameraComponent>(view.front()).camera;
        }

        return m_camera;
    }

    void Scene::onEvent(Event& event)
    {
        {
            auto view = m_registry.view<NativeScriptComponent>();
            for (auto entity : view)
            {
                auto& nsc = view.get<NativeScriptComponent>(entity);
                nsc.instance->onEvent(event);
            }
        }
    }

    void Scene::onUpdate(float dt)
    {
        {
            auto view = m_registry.view<NativeScriptComponent>();
            for (auto entity : view)
            {
                auto& nsc = view.get<NativeScriptComponent>(entity);
                nsc.instance->onUpdate(dt);
            }
        }
        {
            auto group = m_registry.group<CameraComponent>(entt::get<TransformComponent>);
            if (group.size() == 0) {
                return;
            }

            auto [cc, tc] = group.get<CameraComponent, TransformComponent>(*group.begin());
            Renderer2D::beginScene(cc.camera.getVP(tc.getTransform()));
        }
        {
            auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
            for (auto entity : group)
            {
                auto [tc, sc] = group.get<TransformComponent, SpriteComponent>(entity);

                Renderer2D::fillQuad(tc.getTransform(), sc.color);
            }
        }
        Renderer2D::endScene();
    }

} // namespace jng
