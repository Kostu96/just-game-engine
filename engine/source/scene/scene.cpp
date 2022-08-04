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
#include "scripting/lua_script.hpp"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_world.h>

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
            dst.addComponent<Component>() = src.getComponent<Component>();
    }

    static void copyOptionalComponents(Entity dst, Entity src)
    {
        copyComponentIfExists<CameraComponent>(dst, src);
        copyComponentIfExists<CircleRendererComponent>(dst, src);
        copyComponentIfExists<SpriteRendererComponent>(dst, src);
        copyComponentIfExists<BoxCollider2DComponent>(dst, src);
        copyComponentIfExists<CircleCollider2DComponent>(dst, src);
        copyComponentIfExists<Rigidbody2DComponent>(dst, src);
        copyComponentIfExists<LuaScriptComponent>(dst, src);
    }

    Scene::~Scene()
    {
        delete m_physics2dWorld;
    }

    Ref<Scene> Scene::copy(const Ref<Scene>& other)
    {
        Ref<Scene> sceneCopy = makeRef<Scene>();

        other->each([&sceneCopy](Entity entity) {
            std::string tag = entity.getComponent<TagComponent>().Tag;
            GUID id = entity.getComponent<IDComponent>().ID;

            Entity entityCopy = sceneCopy->createEntity(tag, id);
            entityCopy.getComponent<TransformComponent>() = entity.getComponent<TransformComponent>();
            copyOptionalComponents(entityCopy, entity);
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
        entityCopy.getComponent<TransformComponent>() = other.getComponent<TransformComponent>();
        copyOptionalComponents(entityCopy, other);

        return entityCopy;
    }

    void Scene::destroyEntity(Entity entity)
    {
        m_registry.destroy(entity.m_handle);
    }

    void Scene::onCreate()
    {
        m_physics2dWorld = new b2World{ { 0.f, -PHYSICS_GRAVITY_CONSTANT } };

        {
            auto group = m_registry.group<Rigidbody2DComponent>(entt::get<TransformComponent>);
            for (auto entity : group)
            {
                auto [rbc, tc] = group.get<Rigidbody2DComponent, TransformComponent>(entity);

                b2BodyDef bodyDef{};
                bodyDef.type = bodyTypeToBox2DBodyType(rbc.Type);
                bodyDef.position.Set(tc.Translation.x, tc.Translation.y);
                bodyDef.angle = tc.Rotation.z;
                b2Body* body = m_physics2dWorld->CreateBody(&bodyDef);
                body->SetFixedRotation(false);
                rbc.BodyHandle = body;

                Entity jngEntity{ entity, *this };

                if (jngEntity.hasComponent<BoxCollider2DComponent>())
                {
                    auto& bcc = jngEntity.getComponent<BoxCollider2DComponent>();

                    b2PolygonShape shape{};
                    shape.SetAsBox(bcc.Size.x * tc.Scale.x, bcc.Size.y * tc.Scale.y);

                    b2FixtureDef fixtureDef{};
                    fixtureDef.shape = &shape;
                    fixtureDef.density = bcc.Density;
                    fixtureDef.friction = bcc.Friction;
                    fixtureDef.restitution = bcc.Restitution;
                    fixtureDef.restitutionThreshold = bcc.RestitutionThreshold;
                    bcc.FixtureHandle = body->CreateFixture(&fixtureDef);
                }

                if (jngEntity.hasComponent<CircleCollider2DComponent>())
                {
                    auto& ccc = jngEntity.getComponent<CircleCollider2DComponent>();

                    b2CircleShape shape{};
                    shape.m_p.Set(ccc.offset.x, ccc.offset.y);
                    shape.m_radius = ccc.radius * std::max(tc.Scale.x, tc.Scale.y);

                    b2FixtureDef fixtureDef{};
                    fixtureDef.shape = &shape;
                    fixtureDef.density = ccc.Density;
                    fixtureDef.friction = ccc.Friction;
                    fixtureDef.restitution = ccc.Restitution;
                    fixtureDef.restitutionThreshold = ccc.RestitutionThreshold;
                    ccc.FixtureHandle = body->CreateFixture(&fixtureDef);
                }
            }
        }
        {
            auto view = m_registry.view<LuaScriptComponent>();
            for (auto entity : view)
            {
                auto& lsc = view.get<LuaScriptComponent>(entity);
                lsc.instance = LuaScript::create(lsc.path);
                lsc.instance->m_entity = Entity{ entity, *this };
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
                LuaScript::destroy(lsc.instance);
            }
        }

        delete m_physics2dWorld;
        m_physics2dWorld = nullptr;
    }

    void Scene::onEvent(Event& /*event*/)
    {
        /*{
            auto view = m_registry.view<NativeScriptComponent>();
            for (auto entity : view)
            {
                auto& nsc = view.get<NativeScriptComponent>(entity);
                nsc.Instance->onEvent(event);
            }
        }*/
    }

    void Scene::setViewportSize(float width, float height)
    {
        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cc = view.get<CameraComponent>(entity);
            cc.camera.setViewportSize(width, height);
        }
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

    void Scene::onUpdate(float dt)
    {
        /*{
            auto view = m_registry.view<NativeScriptComponent>();
            for (auto entity : view)
            {
                auto& nsc = view.get<NativeScriptComponent>(entity);
                nsc.Instance->onUpdate(dt);
            }
        }*/
        {
            m_physics2dWorld->Step(dt, PHYSICS_VEL_ITERATIONS, PHYSICS_POS_ITERATIONS);

            auto group = m_registry.group<Rigidbody2DComponent>(entt::get<TransformComponent>);
            for (auto entity : group)
            {
                auto [rbc, tc] = group.get<Rigidbody2DComponent, TransformComponent>(entity);

                b2Body* body = reinterpret_cast<b2Body*>(rbc.BodyHandle);
                const auto& pos = body->GetPosition();
                tc.Translation.x = pos.x;
                tc.Translation.y = pos.y;
                tc.Rotation.z = body->GetAngle();
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

} // namespace jng
