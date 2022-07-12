/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <glm/glm.hpp>

namespace jng {

    struct CameraComponent
    {
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        Camera camera;
    };

    struct SpriteComponent
    {
        SpriteComponent() = default;
        SpriteComponent(const SpriteComponent&) = default;

        glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
    };

    struct TagComponent
    {
        TagComponent(const std::string& name) : tag{ name } {};
        TagComponent(const TagComponent&) = default;

        std::string tag;
    };

    struct TransformComponent
    {
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        glm::mat4 transform{ 1.f };
    };

} // namespace jng
