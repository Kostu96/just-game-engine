/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/renderer/texture.hpp"
#include "jng/scene/camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <type_traits>

namespace jng {

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

        glm::vec3 translation{ 0.f, 0.f, 0.f };
        glm::vec3 rotation{ 0.f, 0.f, 0.f };
        glm::vec3 scale{ 1.f, 1.f, 1.f };

        glm::mat4 getTransform() const
        {
            glm::mat4 rotMatrix = glm::rotate(glm::mat4{ 1.f }, glm::radians(rotation.x), { 1.f, 0.f, 0.f }) *
                glm::rotate(glm::mat4{ 1.f }, glm::radians(rotation.y), { 0.f, 1.f, 0.f }) *
                glm::rotate(glm::mat4{ 1.f }, glm::radians(rotation.z), { 0.f, 0.f, 1.f });

            return glm::translate(glm::mat4{ 1.f }, translation) * rotMatrix * glm::scale(glm::mat4{ 1.f }, scale);
        }
    };

    struct CameraComponent
    {
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        Camera camera;
    };

    class NativeScript;

    struct NativeScriptComponent
    {
        NativeScriptComponent() = default;
        NativeScriptComponent(const NativeScriptComponent&) = default;

        NativeScript* instance = nullptr;

        NativeScript* (*createScript)() = nullptr;
        void (*destroyScript)(NativeScript*&) = nullptr;

        template<typename Script>
        void bind()
        {
            static_assert(std::is_base_of_v<NativeScript, Script>);

            createScript = []() -> NativeScript* { return new Script{}; };
            destroyScript = [](NativeScript*& instance) { delete instance; instance = nullptr; };
        }
    };

    struct SpriteComponent
    {
        SpriteComponent() = default;
        SpriteComponent(const SpriteComponent&) = default;

        glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
        Ref<Texture> texture;
    };

    struct BoxCollider2DComponent
    {
        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

        glm::vec2 Size{ 0.5f, 0.5f };
        float Density = 1.f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;
        void* FixtureHandle = nullptr;
    };

    struct Rigidbody2DComponent
    {
        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;

        enum class BodyType { Static, Dynamic, Kinematic };

        BodyType Type = BodyType::Static;
        void* BodyHandle = nullptr;
    };

} // namespace jng
