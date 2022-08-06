/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/GUID.hpp"
#include "jng/renderer/texture.hpp"
#include "jng/scene/camera.hpp"
#include "jng/scripting/lua_engine.hpp"

#include <glm/glm.hpp>
#include <string>
#include <type_traits>

class b2Body;

namespace jng {

    struct IDComponent
    {
        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
        IDComponent(GUID id) : ID{ id } {}

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

        void reset();
        glm::mat4 getTransform() const;
    };

    struct CameraComponent
    {
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        Camera camera;
        
        void reset();
    };

    struct CircleRendererComponent
    {
        CircleRendererComponent() = default;
        CircleRendererComponent(const CircleRendererComponent&) = default;

        glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
        float thickness = 1.f;
        float fade = 0.001f;

        void reset();
    };

    struct SpriteRendererComponent
    {
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;

        glm::vec4 Color{ 1.f, 1.f, 1.f, 1.f };
        Ref<Texture> texture;

        void reset();
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

        void reset();
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

        void reset();
    };

    struct Rigidbody2DComponent
    {
        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;

        enum class BodyType { Static = 0, Kinematic = 1, Dynamic = 2 };

        BodyType Type = BodyType::Static;
        bool freezeRotation = false;
        float linearDamping = 0.1f;
        float angularDamping = 0.1f;

        b2Body* BodyHandle = nullptr; // NOTE: used in runtime only

        void reset();
        void setLinearVelocity(glm::vec2 velocity);
    };

    struct LuaScriptComponent
    {
        LuaScriptComponent() = default;
        LuaScriptComponent(const LuaScriptComponent&) = default;

        std::string name;
        LuaEngine::ScriptData data;

        void reset();
    };

    template<typename... Component>
    struct ComponentGroup {};

    // All components except ID, Tag and Transform
    using OptionalComponents = ComponentGroup<
        CameraComponent,
        CircleRendererComponent,
        SpriteRendererComponent,
        BoxCollider2DComponent,
        CircleCollider2DComponent,
        Rigidbody2DComponent,
        LuaScriptComponent
    >;

} // namespace jng
