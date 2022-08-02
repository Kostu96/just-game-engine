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
#include "scripting/native_script.hpp"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
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

    Scene::~Scene()
    {
        delete m_physics2dWorld;
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
            }
        }
        {
            auto view = m_registry.view<NativeScriptComponent>();
            for (auto entity : view)
            {
                auto& nsc = view.get<NativeScriptComponent>(entity);
                nsc.Instance = nsc.createScript();
                nsc.Instance->m_entity = Entity{ entity, *this };
                nsc.Instance->onCreate();
            }
        }
    }

    void Scene::onDestroy()
    {
        {
            auto view = m_registry.view<NativeScriptComponent>();
            for (auto entity : view)
            {
                auto& nsc = view.get<NativeScriptComponent>(entity);
                nsc.Instance->onDestroy();
                nsc.destroyScript(nsc.Instance);
            }
        }

        delete m_physics2dWorld;
        m_physics2dWorld = nullptr;
    }

    void Scene::onEvent(Event& event)
    {
        {
            auto view = m_registry.view<NativeScriptComponent>();
            for (auto entity : view)
            {
                auto& nsc = view.get<NativeScriptComponent>(entity);
                nsc.Instance->onEvent(event);
            }
        }
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

    void Scene::drawSprites()
    {
        auto group = m_registry.group<SpriteComponent>(entt::get<TransformComponent>);
        for (auto entity : group)
        {
            auto [sc, tc] = group.get<SpriteComponent, TransformComponent>(entity);
            Renderer2D::drawSprite(tc.getTransform(), sc, static_cast<int32>(entity));
        }
    }

    void Scene::onUpdate(float dt)
    {
        {
            auto view = m_registry.view<NativeScriptComponent>();
            for (auto entity : view)
            {
                auto& nsc = view.get<NativeScriptComponent>(entity);
                nsc.Instance->onUpdate(dt);
            }
        }
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
        drawSprites();
        Renderer2D::endScene();
    }

} // namespace jng
