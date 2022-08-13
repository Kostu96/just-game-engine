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

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_world.h>
#include <glm/gtc/matrix_transform.hpp>

namespace jng {

    static b2BodyType bodyTypeToBox2DBodyType(Rigidbody2DComponent::BodyType type)
    {
        switch (type)
        {
        case Rigidbody2DComponent::BodyType::Static: return b2BodyType::b2_staticBody;
        case Rigidbody2DComponent::BodyType::Dynamic: return b2BodyType::b2_dynamicBody;
        case Rigidbody2DComponent::BodyType::Kinematic: return b2BodyType::b2_kinematicBody;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return static_cast<b2BodyType>(-1);
    }

    template<typename Component>
    static void copyComponentIfExists(Entity dst, Entity src)
    {
        if (src.hasComponent<Component>())
            dst.addOrReplaceComponent<Component>(src.getComponent<Component>());
    }

    template<typename... Component>
    static void copyComponents(ComponentGroup<Component...>, Entity dst, Entity src)
    {
        ([&]() { copyComponentIfExists<Component>(dst, src); }(), ...);
    }

    Scene::~Scene()
    {
        delete m_physics2dWorld;
    }

    Ref<Scene> Scene::copy(const Ref<Scene>& other)
    {
        Ref<Scene> sceneCopy = makeRef<Scene>();

        sceneCopy->m_viewportWidth = other->m_viewportWidth;
        sceneCopy->m_viewportHeight = other->m_viewportHeight;

        other->each([&sceneCopy](Entity entity) {
            std::string tag = entity.getComponent<TagComponent>().Tag;
            GUID id = entity.getComponent<IDComponent>().ID;

            Entity entityCopy = sceneCopy->createEntity(tag, id);
            copyComponents(AllComponents{}, entityCopy, entity);
        });

        return sceneCopy;
    }

    Entity Scene::createEntity(const std::string& name)
    {
        auto entity = m_registry.create();
        m_registry.emplace<IDComponent>(entity);
        m_registry.emplace<TagComponent>(entity, name);
        m_registry.emplace<TransformComponent>(entity);

        return Entity{ entity, *this };
    }

    Entity Scene::createEntity(const std::string& name, GUID id)
    {
        auto entity = m_registry.create();
        m_registry.emplace<IDComponent>(entity, id);
        m_registry.emplace<TagComponent>(entity, name);
        m_registry.emplace<TransformComponent>(entity);

        return Entity{ entity, *this };
    }

    Entity Scene::duplicateEntity(Entity other)
    {
        std::string tag = other.getComponent<TagComponent>().Tag + " Copy";
        Entity entityCopy = createEntity(tag);
        copyComponents(AllComponents{}, entityCopy, other);

        return entityCopy;
    }

    void Scene::destroyEntity(Entity entity)
    {
        m_registry.destroy(entity.m_handle);
    }

    void Scene::onCreate(f32 gravity)
    {
        {
            auto view = m_registry.view<CameraComponent>();
            for (auto entity : view)
            {
                auto& cc = view.get<CameraComponent>(entity);
                cc.camera.setViewportSize(m_viewportWidth, m_viewportHeight);
            }
        }

        {
            auto view = m_registry.view<LuaScriptComponent>();
            for (auto entity : view)
            {
                auto& lsc = view.get<LuaScriptComponent>(entity);
                LuaEngine::onCreate(Entity{ entity, *this }, lsc);
            }
        }

        m_physics2dWorld = new b2World{ { 0.f, -gravity } };
        {
            auto group = m_registry.group<Rigidbody2DComponent>(entt::get<TransformComponent>);
            for (auto entity : group)
            {
                auto [rbc, tc] = group.get<Rigidbody2DComponent, TransformComponent>(entity);

                b2BodyDef bodyDef{};
                bodyDef.type = bodyTypeToBox2DBodyType(rbc.type);
                bodyDef.enabled = rbc.enabled;
                bodyDef.position.Set(tc.translation.x, tc.translation.y);
                bodyDef.angle = tc.rotation.z;
                bodyDef.fixedRotation = rbc.freezeRotation;
                bodyDef.linearDamping = rbc.linearDamping;
                bodyDef.angularDamping = rbc.angularDamping;
                bodyDef.gravityScale = rbc.gravityScale;
                rbc.bodyHandle = m_physics2dWorld->CreateBody(&bodyDef);

                Entity jngEntity{ entity, *this };

                if (jngEntity.hasComponent<BoxCollider2DComponent>())
                {
                    auto& bcc = jngEntity.getComponent<BoxCollider2DComponent>();

                    b2PolygonShape shape{};
                    shape.SetAsBox(bcc.size.x * tc.scale.x, bcc.size.y * tc.scale.y, { bcc.offset.x, bcc.offset.y }, 0.f);

                    b2FixtureDef fixtureDef{};
                    fixtureDef.shape = &shape;
                    fixtureDef.density = bcc.density;
                    fixtureDef.friction = bcc.friction;
                    fixtureDef.restitution = bcc.restitution;
                    fixtureDef.restitutionThreshold = bcc.restitutionThreshold;
                    bcc.fixtureHandle = rbc.bodyHandle->CreateFixture(&fixtureDef);
                }

                if (jngEntity.hasComponent<CircleCollider2DComponent>())
                {
                    auto& ccc = jngEntity.getComponent<CircleCollider2DComponent>();

                    b2CircleShape shape{};
                    shape.m_p.Set(ccc.offset.x, ccc.offset.y);
                    shape.m_radius = ccc.radius * std::max(tc.scale.x, tc.scale.y);

                    b2FixtureDef fixtureDef{};
                    fixtureDef.shape = &shape;
                    fixtureDef.density = ccc.density;
                    fixtureDef.friction = ccc.friction;
                    fixtureDef.restitution = ccc.restitution;
                    fixtureDef.restitutionThreshold = ccc.restitutionThreshold;
                    ccc.fixtureHandle = rbc.bodyHandle->CreateFixture(&fixtureDef);
                }
            }
        }
    }

    void Scene::onDestroy()
    {
        {
            auto view = m_registry.view<LuaScriptComponent>();
            for (auto entity : view)
            {
                auto& lsc = view.get<LuaScriptComponent>(entity);
                LuaEngine::onDestroy(Entity{ entity, *this }, lsc);
            }
        }

        delete m_physics2dWorld;
        m_physics2dWorld = nullptr;
    }

    void Scene::onUpdate(f32 dt)
    {
        {
            auto view = m_registry.view<LuaScriptComponent>();
            for (auto entity : view)
            {
                auto& lsc = view.get<LuaScriptComponent>(entity);
                LuaEngine::onUpdate(Entity{ entity, *this }, lsc, dt);
            }
        }
        {
            m_physics2dWorld->Step(dt, PHYSICS_VEL_ITERATIONS, PHYSICS_POS_ITERATIONS);

            auto group = m_registry.group<Rigidbody2DComponent>(entt::get<TransformComponent>);
            for (auto entity : group)
            {
                auto [rbc, tc] = group.get<Rigidbody2DComponent, TransformComponent>(entity);

                b2Body* body = reinterpret_cast<b2Body*>(rbc.bodyHandle);
                const auto& pos = body->GetPosition();
                tc.translation.x = pos.x;
                tc.translation.y = pos.y;
                tc.rotation.z = body->GetAngle();
            }
        }
        {
            auto group = m_registry.group<CameraComponent>(entt::get<TransformComponent>);
            if (group.size() == 0) {
                JNG_CORE_WARN("Scene has no camera!");
                return;
            }

            auto [cc, tc] = group.get<CameraComponent, TransformComponent>(*group.begin());
            Renderer2D::beginScene(cc.camera.getVP(tc.getTransform()));
        }
        drawRenderables();
        Renderer2D::endScene();
    }

    void Scene::onEvent(Event& /*event*/)
    {
        
    }

    void Scene::drawRenderables()
    {
        auto spriteGroup = m_registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
        for (auto entity : spriteGroup)
        {
            auto [src, tc] = spriteGroup.get<SpriteRendererComponent, TransformComponent>(entity);
            Renderer2D::drawSprite(tc.getTransform(), src, static_cast<int32>(entity));
        }

        auto circleGroup = m_registry.group<CircleRendererComponent>(entt::get<TransformComponent>);
        for (auto entity : circleGroup)
        {
            auto [crc, tc] = circleGroup.get<CircleRendererComponent, TransformComponent>(entity);
            Renderer2D::drawCircle(tc.getTransform(), crc, static_cast<int32>(entity));
        }
    }

    void Scene::drawColliders()
    {
        {
            auto group = m_registry.group<BoxCollider2DComponent>(entt::get<TransformComponent>);
            for (auto entity : group)
            {
                auto [bcc, tc] = group.get<BoxCollider2DComponent, TransformComponent>(entity);
                b2PolygonShape shape{};
                shape.SetAsBox(
                    bcc.size.x * tc.scale.x, bcc.size.y * tc.scale.y,
                    b2Vec2{ tc.translation.x, tc.translation.y } + b2Vec2{ bcc.offset.x, bcc.offset.y },
                    tc.rotation.z
                );
                
                b2Vec2 p1 = shape.m_vertices[shape.m_count - 1];
                for (int32 i = 0; i < shape.m_count; ++i)
                {
                    b2Vec2 p2 = shape.m_vertices[i];
                    Renderer2D::drawLine(
                        { p1.x, p1.y, 0.01f },
                        { p2.x, p2.y, 0.01f },
                        { 0.25f, 1.f, 0.f, 1.f }
                    );
                    p1 = p2;
                }
            }
        }
        {
            auto group = m_registry.group<CircleCollider2DComponent>(entt::get<TransformComponent>);
            for (auto entity : group)
            {
                auto [ccc, tc] = group.get<CircleCollider2DComponent, TransformComponent>(entity);
                b2Vec2 center{ ccc.offset.x, ccc.offset.y };
                float radius = ccc.radius * std::max(tc.scale.x, tc.scale.y);
                const float k_segments = 16.0f;
                const float k_increment = 2.0f * b2_pi / k_segments;
                float sinInc = sinf(k_increment);
                float cosInc = cosf(k_increment);
                b2Vec2 r1(1.0f, 0.0f);
                b2Vec2 v1 = center + radius * r1;
                for (int32 i = 0; i < k_segments; ++i)
                {
                    // Perform rotation to avoid additional trigonometry.
                    b2Vec2 r2;
                    r2.x = cosInc * r1.x - sinInc * r1.y;
                    r2.y = sinInc * r1.x + cosInc * r1.y;
                    b2Vec2 v2 = center + radius * r2;
                    Renderer2D::drawLine(
                        { v1.x, v1.y, 0.01f },
                        { v2.x, v2.y, 0.01f },
                        { 0.25f, 1.f, 0.f, 1.f }
                    );
                    r1 = r2;
                    v1 = v2;
                }
            }
        }
    }

} // namespace jng
