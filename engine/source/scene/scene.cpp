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

        other->each([&sceneCopy](Entity entity) {
            if (!entity.hasParent())
                copyEntityWithChildren(sceneCopy, entity);
        });

        return sceneCopy;
    }

    Entity Scene::createEntity(const std::string& name)
    {
        auto entity = m_registry.create();
        m_registry.emplace<IDComponent>(entity);
        m_registry.emplace<TagComponent>(entity, name);
        m_registry.emplace<WorldTransformComponent>(entity);

        return Entity{ entity, *this };
    }

    Entity Scene::createEntity(const std::string& name, GUID id)
    {
        auto entity = m_registry.create();
        m_registry.emplace<IDComponent>(entity, id);
        m_registry.emplace<TagComponent>(entity, name);
        m_registry.emplace<WorldTransformComponent>(entity);

        return Entity{ entity, *this };
    }

    Entity Scene::duplicateEntity(Entity other)
    {
        // TODO: duplicate children and assign the same parent

        std::string tag = other.getComponent<TagComponent>().Tag + " Copy";
        Entity entityCopy = createEntity(tag);
        copyComponents(AllComponents{}, entityCopy, other);

        return entityCopy;
    }

    void Scene::destroyEntity(Entity entity)
    {
        // TODO: destroy children and remove from parents children

        m_registry.destroy(entity.m_handle);
    }

    void Scene::onCreate(float gravity)
    {
        calculateWorldTransforms();

        m_physics2dWorld = new b2World{ { 0.f, -gravity } };
        {
            auto group = m_registry.group<Rigidbody2DComponent>(entt::get<WorldTransformComponent>);
            for (auto entity : group)
            {
                auto [rbc, tc] = group.get<Rigidbody2DComponent, WorldTransformComponent>(entity);

                b2BodyDef bodyDef{};
                bodyDef.type = bodyTypeToBox2DBodyType(rbc.type);
                bodyDef.enabled = rbc.enabled;
                bodyDef.position.Set(tc.Translation.x, tc.Translation.y);
                bodyDef.angle = tc.Rotation.z;
                bodyDef.fixedRotation = rbc.freezeRotation;
                bodyDef.linearDamping = rbc.linearDamping;
                bodyDef.angularDamping = rbc.angularDamping;
                rbc.bodyHandle = m_physics2dWorld->CreateBody(&bodyDef);

                Entity jngEntity{ entity, *this };

                if (jngEntity.hasComponent<BoxCollider2DComponent>())
                {
                    auto& bcc = jngEntity.getComponent<BoxCollider2DComponent>();

                    b2PolygonShape shape{};
                    shape.SetAsBox(bcc.size.x * tc.Scale.x, bcc.size.y * tc.Scale.y, { bcc.offset.x, bcc.offset.y }, 0.f);

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
                    shape.m_radius = ccc.radius * std::max(tc.Scale.x, tc.Scale.y);

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
        {
            auto view = m_registry.view<LuaScriptComponent>();
            for (auto entity : view)
            {
                auto& lsc = view.get<LuaScriptComponent>(entity);
                LuaEngine::onCreate(Entity{ entity, *this }, lsc);
            }
        }
    }

    void Scene::onDestroy()
    {
        delete m_physics2dWorld;
        m_physics2dWorld = nullptr;
    }

    void Scene::onUpdate(float dt)
    {
        calculateWorldTransforms();

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

            auto group = m_registry.group<Rigidbody2DComponent>(entt::get<WorldTransformComponent>);
            for (auto entity : group)
            {
                auto [rbc, tc] = group.get<Rigidbody2DComponent, WorldTransformComponent>(entity);

                b2Body* body = reinterpret_cast<b2Body*>(rbc.bodyHandle);
                const auto& pos = body->GetPosition();
                tc.Translation.x = pos.x;
                tc.Translation.y = pos.y;
                tc.Rotation.z = body->GetAngle();
            }
        }
        {
            auto group = m_registry.group<CameraComponent>(entt::get<WorldTransformComponent>);
            if (group.size() == 0) {
                JNG_CORE_WARN("Scene has no camera!");
                return;
            }

            auto [cc, tc] = group.get<CameraComponent, WorldTransformComponent>(*group.begin());
            Renderer2D::beginScene(cc.camera.getVP(tc.getTransform()));
        }
        drawRenderables();
        Renderer2D::endScene();
    }

    void Scene::onEvent(Event& /*event*/)
    {
        
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

    static void setChildrenDirty(Entity entity)
    {
        if (entity.hasChildren())
        {
            auto& children = entity.getComponent<ChildrenComponent>().children;
            for (auto child : children)
            {
                child.getComponent<WorldTransformComponent>().isDirty = true;
                setChildrenDirty(child);
            }
        }
    }

    static WorldTransformComponent& calculateParentTransform(Entity entity)
    {
        auto parent = entity.getComponent<ParentComponent>().parent;
        auto& parentTransform = parent.getComponent<WorldTransformComponent>();
        if (parentTransform.isDirty)
        {
            auto& grandParentTransform = calculateParentTransform(parent);
            parentTransform.setTransform(grandParentTransform.getTransform() * parentTransform.getTransform());
        }

        return parentTransform;
    }

    void Scene::calculateWorldTransforms()
    {
        auto ltcWithWtcGroup = m_registry.group<LocalTransformComponent>(entt::get<WorldTransformComponent>);
        for (auto entity : ltcWithWtcGroup)
            ltcWithWtcGroup.get<WorldTransformComponent>(entity).isDirty = ltcWithWtcGroup.get<LocalTransformComponent>(entity).isDirty;

        {
            auto view = m_registry.view<WorldTransformComponent>();
            for (auto entity : view)
                if (view.get<WorldTransformComponent>(entity).isDirty)
                    setChildrenDirty({ entity, *this });
        }
        
        for (auto entity : ltcWithWtcGroup)
        {
            auto [ltc, wtc] = ltcWithWtcGroup.get<LocalTransformComponent, WorldTransformComponent>(entity);
            if (wtc.isDirty)
            {
                auto& parentTransform = calculateParentTransform({ entity, *this });
                wtc.setTransform(parentTransform.getTransform() * ltc.getTransform());
                ltc.isDirty = false;
                wtc.isDirty = false;
            }
        }
    }

    Entity Scene::copyEntityWithChildren(Ref<Scene>& scene, Entity entity)
    {
        std::string tag = entity.getComponent<TagComponent>().Tag;
        GUID id = entity.getComponent<IDComponent>().ID;

        Entity entityCopy = scene->createEntity(tag, id);
        copyComponents(AllComponents{}, entityCopy, entity);
        copyChildren(scene, entityCopy, entity);

        return entityCopy;
    }

    void Scene::copyChildren(Ref<Scene>& scene, Entity dst, Entity src)
    {
        if (src.hasChildren())
        {
            auto& children = src.getComponent<ChildrenComponent>();
            for (auto child : children.children)
            {
                Entity childCopy = copyEntityWithChildren(scene, child);
                childCopy.setParent(dst);
            }
        }
    }

    void Scene::drawRenderables()
    {
        auto spriteGroup = m_registry.group<SpriteRendererComponent>(entt::get<WorldTransformComponent>);
        for (auto entity : spriteGroup)
        {
            auto [src, tc] = spriteGroup.get<SpriteRendererComponent, WorldTransformComponent>(entity);
            Renderer2D::drawSprite(tc.getTransform(), src, static_cast<int32>(entity));
        }

        auto circleGroup = m_registry.group<CircleRendererComponent>(entt::get<WorldTransformComponent>);
        for (auto entity : circleGroup)
        {
            auto [crc, tc] = circleGroup.get<CircleRendererComponent, WorldTransformComponent>(entity);
            Renderer2D::drawCircle(tc.getTransform(), crc, static_cast<int32>(entity));
        }
    }

    void Scene::drawColliders()
    {
        {
            auto group = m_registry.group<BoxCollider2DComponent>(entt::get<WorldTransformComponent>);
            for (auto entity : group)
            {
                auto [bcc, tc] = group.get<BoxCollider2DComponent, WorldTransformComponent>(entity);
                glm::vec3 translation = tc.Translation;
                glm::vec3 scale = tc.Scale;
                glm::mat4 transform = glm::translate(glm::mat4{ 1.f }, translation) * glm::scale(glm::mat4{ 1.f }, scale);
                Renderer2D::drawRect(transform, { 0.25f, 1.f, 0.f, 1.f });
            }
        }
        {
            auto group = m_registry.group<CircleCollider2DComponent>(entt::get<WorldTransformComponent>);
            for (auto entity : group)
            {
                auto [ccc, tc] = group.get<CircleCollider2DComponent, WorldTransformComponent>(entity);
                glm::vec3 translation = tc.Translation + glm::vec3{ ccc.offset, 0.001f };
                glm::vec3 scale = tc.Scale * ccc.radius * 2.f;
                glm::mat4 transform = glm::translate(glm::mat4{ 1.f }, translation) * glm::scale(glm::mat4{ 1.f }, scale);
                Renderer2D::drawCircle(transform, { 0.25f, 1.f, 0.f, 1.f }, 0.05f);
            }
        }
    }

} // namespace jng
