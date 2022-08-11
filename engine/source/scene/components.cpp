/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/scene/components.hpp"

#include <box2d/b2_body.h>

namespace jng {

    void CameraComponent::reset()
    {
        camera.reset();
    }

    void CircleRendererComponent::reset()
    {
        color = { 1.f, 1.f, 1.f, 1.f };
        thickness = 1.f;
        fade = 0.001f;
    }

    void SpriteRendererComponent::reset()
    {
        Color = { 1.f, 1.f, 1.f, 1.f };
        texture = {};
    }

    void BoxCollider2DComponent::reset()
    {
        size = { 0.5f, 0.5f };
        offset = { 0.f, 0.f };
        density = 1.f;
        friction = 0.5f;
        restitution = 0.0f;
        restitutionThreshold = 0.5f;
    }

    void CircleCollider2DComponent::reset()
    {
        radius = 0.5f;
        offset = { 0.f, 0.f };
        density = 1.f;
        friction = 0.5f;
        restitution = 0.0f;
        restitutionThreshold = 0.5f;
    }

    void Rigidbody2DComponent::reset()
    {
        type = BodyType::Static;
        enabled = true;
        freezeRotation = false;
        linearDamping = 0.1f;
        angularDamping = 0.1f;
        gravityScale = 1.f;
    }

    void Rigidbody2DComponent::setLinearVelocity(glm::vec2 velocity)
    {
        bodyHandle->SetLinearVelocity({ velocity.x, velocity.y });
    }

    void LuaScriptComponent::reset()
    {
        name = {};
    }

} // namespace jng
