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
#include "jng/utilities/math.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

class b2Body;
class b2Fixture;

namespace jng {

    struct IDComponent
    {
        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
        IDComponent(GUID id) : ID{ id } {}

        GUID ID;
    };
    static_assert(sizeof(IDComponent) == 8);

    struct TagComponent
    {
        TagComponent(const std::string& tag) : Tag{ tag } {};
        TagComponent(const TagComponent&) = default;

        std::string Tag;
    };
    static_assert(sizeof(TagComponent) == 40);

    struct TransformComponent
    {
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        glm::vec3 translation{ 0.f, 0.f, 0.f };
        glm::vec3 rotation{ 0.f, 0.f, 0.f };
        glm::vec3 scale{ 1.f, 1.f, 1.f };

        void reset();

        void setTransform(const glm::mat4& transform);
        glm::mat4 getTransform() const;
    };
    static_assert(sizeof(TransformComponent) == 36);

    struct CameraComponent
    {
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        Camera camera;
        
        void reset();
    };
    static_assert(sizeof(CameraComponent) == 96);

    struct CircleRendererComponent
    {
        CircleRendererComponent() = default;
        CircleRendererComponent(const CircleRendererComponent&) = default;

        glm::vec4 color{ 1.f, 1.f, 1.f, 1.f };
        f32 thickness = 1.f;
        f32 fade = 0.001f;

        void reset();
    };
    static_assert(sizeof(CircleRendererComponent) == 24);

    struct SpriteRendererComponent
    {
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;

        glm::vec4 Color{ 1.f, 1.f, 1.f, 1.f };
        Ref<Texture> texture;

        void reset();
    };
    static_assert(sizeof(SpriteRendererComponent) == 32);

    struct BoxCollider2DComponent
    {
        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

        b2Fixture* fixtureHandle = nullptr; // NOTE: used in runtime only
        glm::vec2 size{ 0.5f, 0.5f };
        glm::vec2 offset{ 0.f, 0.f };
        f32 density = 1.f;
        f32 friction = 0.5f;
        f32 restitution = 0.0f;
        f32 restitutionThreshold = 0.5f;

        void reset();
    };
    static_assert(sizeof(BoxCollider2DComponent) == 40);

    struct CircleCollider2DComponent
    {
        CircleCollider2DComponent() = default;
        CircleCollider2DComponent(const CircleCollider2DComponent&) = default;

        b2Fixture* fixtureHandle = nullptr; // NOTE: used in runtime only
        glm::vec2 offset{ 0.f, 0.f };
        f32 radius = 0.5f;
        f32 density = 1.f;
        f32 friction = 0.5f;
        f32 restitution = 0.0f;
        f32 restitutionThreshold = 0.5f;
        u32 __padding; // NOTE: padding because struct is aligned so that struct size is divisivle by the size of the largest member.

        void reset();
    };
    static_assert(sizeof(CircleCollider2DComponent) == 40);

    struct Rigidbody2DComponent
    {
        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;

        enum class BodyType { Static = 0, Kinematic = 1, Dynamic = 2 };

        b2Body* bodyHandle = nullptr; // NOTE: used in runtime only
        BodyType type = BodyType::Static;
        f32 linearDamping = 0.1f;
        f32 angularDamping = 0.1f;
        f32 gravityScale = 1.f;
        bool enabled = true;
        bool freezeRotation = false;

        void reset();
        void setLinearVelocity(glm::vec2 velocity);
    };
    static_assert(sizeof(Rigidbody2DComponent) == 32);

    struct LuaScriptComponent
    {
        LuaScriptComponent() = default;
        LuaScriptComponent(const LuaScriptComponent&) = default;

        std::string name;
        LuaEngine::ScriptData data;

        void reset();
    };
    static_assert(sizeof(LuaScriptComponent) == 72);

    template<typename... Component>
    struct ComponentGroup {};

    // All components except ID, Tag
    using AllComponents = ComponentGroup<
        TransformComponent,
        CameraComponent,
        CircleRendererComponent,
        SpriteRendererComponent,
        BoxCollider2DComponent,
        CircleCollider2DComponent,
        Rigidbody2DComponent,
        LuaScriptComponent
    >;

} // namespace jng
