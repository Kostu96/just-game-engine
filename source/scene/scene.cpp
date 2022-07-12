/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scene/scene.hpp"

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
        auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(transform.transform, scale, rotation, translation, skew, perspective);
            Renderer2D::fillQuad(glm::vec2{ translation.x, translation.y }, glm::vec2{ scale.x, scale.y }, sprite.color);
        }
    }

} // namespace jng
