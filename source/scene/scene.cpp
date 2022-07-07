/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scene/scene.hpp"

#include "scene/components.hpp"
#include "scene/entity.hpp"

namespace jng {

    Entity Scene::createEntity()
    {
        auto entity = m_registry.create();
        m_registry.emplace<TagComponent>(entity);
        m_registry.emplace<TransformComponent>(entity);

        return Entity(entity, *this);
    }

    void Scene::onUpdate()
    {

    }

} // namespace jng
