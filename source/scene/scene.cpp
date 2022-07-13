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

    void Scene::onUpdate()
    {
        {
            auto group = m_registry.group<CameraComponent>(entt::get<TransformComponent>);
            if (group.size() == 0) {
                //JNG_CORE_WARN("Scene has no camera!");
                return; // TODO: temp
            }
            else if (group.size() > 1)
                JNG_CORE_WARN("More than one camera on the scene.");

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
