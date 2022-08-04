/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/GUID.hpp"
#include "jng/renderer/texture.hpp"
#include "jng/scene/camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <filesystem>
#include <string>
#include <type_traits>

namespace jng {

    struct IDComponent
    {
        IDComponent() = default;
        IDComponent(GUID id) : ID{ id } {}
        IDComponent(const IDComponent&) = default;

        GUID ID;
    };

    struct TagComponent
    {
        TagComponent(const std::string& tag) : Tag{ tag } {};
        TagComponent(const TagComponent&) = default;

        std::string Tag;
    };

    struct TransformComponent
    {
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        glm::vec3 Translation{ 0.f, 0.f, 0.f };
        glm::vec3 Rotation{ 0.f, 0.f, 0.f };
        glm::vec3 Scale{ 1.f, 1.f, 1.f };

        void reset()
        {
            Translation = { 0.f, 0.f, 0.f };
            Rotation = { 0.f, 0.f, 0.f };
            Scale = { 1.f, 1.f, 1.f };
        }

        glm::mat4 getTransform() const
        {
            glm::mat4 rotMatrix = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4{ 1.f }, Translation) * rotMatrix * glm::scale(glm::mat4{ 1.f }, Scale);
        }
    };

    struct CameraComponent
    {
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        Camera camera;
        
        void reset()
        {
            camera.reset();
        }
    };

    struct CircleRendererComponent
    {
        CircleRendererComponent() = default;
        CircleRendererComponent(const CircleRendererComponent&) = default;

        glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
        float thickness = 1.f;
        float fade = 0.001f;

        void reset()
        {
            color = { 1.f, 1.f, 1.f, 1.f };
            thickness = 1.f;
            fade = 0.001f;
        }
    };

    struct SpriteRendererComponent
    {
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;

        glm::vec4 Color{ 1.f, 1.f, 1.f, 1.f };
        Ref<Texture> texture;

        void reset()
        {
            Color = { 1.f, 1.f, 1.f, 1.f };
            texture = {};
        }
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

        void* FixtureHandle = nullptr; // NOTE: used in runtime only

        void reset()
        {
            Size = { 0.5f, 0.5f };
            Density = 1.f;
            Friction = 0.5f;
            Restitution = 0.0f;
            RestitutionThreshold = 0.5f;
        }
    };

    struct CircleCollider2DComponent
    {
        CircleCollider2DComponent() = default;
        CircleCollider2DComponent(const CircleCollider2DComponent&) = default;

        float radius = 0.5f;
        glm::vec2 offset{ 0.f, 0.f };
        float Density = 1.f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        void* FixtureHandle = nullptr; // NOTE: used in runtime only

        void reset()
        {
            radius = 0.5f;
            offset = { 0.f, 0.f };
            Density = 1.f;
            Friction = 0.5f;
            Restitution = 0.0f;
            RestitutionThreshold = 0.5f;
        }
    };

    struct Rigidbody2DComponent
    {
        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;

        enum class BodyType { Static = 0, Dynamic = 1, Kinematic = 2 };

        BodyType Type = BodyType::Static;

        void* BodyHandle = nullptr; // NOTE: used in runtime only

        void reset()
        {
            Type = BodyType::Static;
        }
    };

    class LuaScript;

    struct LuaScriptComponent
    {
        LuaScriptComponent() = default;
        LuaScriptComponent(const LuaScriptComponent&) = default;

        std::filesystem::path path;
        Ref<LuaScript> instance;

        void reset()
        {
            path = std::filesystem::path{};
            instance = {};
        }
    };

} // namespace jng
