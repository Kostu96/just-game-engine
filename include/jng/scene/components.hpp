/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <glm/glm.hpp>

namespace jng {

    struct TagComponent
    {
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;

        std::string tag{ "Unnamed" };
    };

    struct TransformComponent
    {
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        glm::mat4 transform{ 1.f };
    };

    struct SpriteComponent
    {
        SpriteComponent() = default;
        SpriteComponent(const SpriteComponent&) = default;

        glm::vec3 color{ 1.f, 1.f, 1.f };
    };

} // namespace jng