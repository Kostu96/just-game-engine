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

    void Scene::onUpdate()
    {
        auto view = m_registry.view<CameraComponent>();
        if (view.size() == 0) {
            //JNG_CORE_WARN("Scene has no camera!");
            return; // TODO: temp
        }
        else if (view.size() > 1)
            JNG_CORE_WARN("More than one camera on the scene.");

        auto& cameraComponent = view.get<CameraComponent>(*view.begin());
        Renderer2D::beginScene(cameraComponent.camera.getVP());

        auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
        for (auto entity : group)
        {
            auto [tc, sc] = group.get<TransformComponent, SpriteComponent>(entity);

            Renderer2D::fillQuad(tc.getTransform(), sc.color);
        }

        Renderer2D::endScene();
    }

} // namespace jng
