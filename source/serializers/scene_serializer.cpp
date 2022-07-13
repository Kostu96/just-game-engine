/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "serializers/scene_serializer.hpp"

#include "scene/components.hpp"
#include "scene/entity.hpp"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace YAML {

    Emitter& operator<<(Emitter& yaml, const glm::vec3& value)
    {
        yaml << Flow;
        yaml << BeginSeq << value.x << value.y << value.z << EndSeq;
        return yaml;
    }

    Emitter& operator<<(Emitter& yaml, const glm::vec4& value)
    {
        yaml << Flow;
        yaml << BeginSeq << value.x << value.y << value.z << value.w << EndSeq;
        return yaml;
    }

    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

} // namespace YAML

namespace jng {

    void SceneSerializer::serialize(const char* filename)
    {
        YAML::Emitter yaml;

        yaml << YAML::BeginMap;

        yaml << YAML::Key << "Scene" << YAML::Value << "Untitled";
        yaml << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        m_scene->each([this, &yaml](Entity entity) { serializeEntity(entity, yaml); });
        yaml << YAML::EndSeq;

        yaml << YAML::EndMap;

        std::ofstream fout{ filename };
        fout << yaml.c_str();
        fout.close();
    }

    void SceneSerializer::deserialize(const char* filename)
    {
        YAML::Node data = YAML::LoadFile(filename);
        if (!data["Scene"]) {
            JNG_CORE_ERROR("Error loading scene file: {0}\nMissing 'Scene' node!", filename);
            return;
        }

        JNG_CORE_TRACE("Deserializing scene: {0}", "Untitled");

        auto entities = data["Entities"];
        if (entities)
            for (auto entity : entities)
            {
                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent) name = tagComponent["Tag"].as<std::string>();

                JNG_CORE_TRACE("  Deserializing entity: {0}", name);

                Entity deserializedEntity = m_scene->createEntity(name);

                auto transformComponent = entity["TransformComponent"];
                if (transformComponent)
                {
                    auto& tc = deserializedEntity.getComponent<TransformComponent>();
                    tc.translation = transformComponent["Translation"].as<glm::vec3>();
                    tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
                    tc.scale = transformComponent["Scale"].as<glm::vec3>();
                }

                auto cameraComponent = entity["CameraComponent"];
                if (cameraComponent)
                {
                    auto& cc = deserializedEntity.addComponent<CameraComponent>();
                    cc.camera.setProjectionType(static_cast<Camera::ProjectionType>(cameraComponent["ProjectionType"].as<int>()));
                    cc.camera.setOrthographicSize(cameraComponent["OrthographicSize"].as<float>());
                    cc.camera.setOrthographicNear(cameraComponent["OrthographicNear"].as<float>());
                    cc.camera.setOrthographicFar(cameraComponent["OrthographicFar"].as<float>());
                    cc.camera.setPerspectiveFOV(cameraComponent["PerspectiveFOV"].as<float>());
                    cc.camera.setPerspectiveNear(cameraComponent["PerspectiveNear"].as<float>());
                    cc.camera.setPerspectiveFar(cameraComponent["PerspectiveFar"].as<float>());
                }

                // TODO: NativeScriptComponent

                auto spriteComponent = entity["SpriteComponent"];
                if (spriteComponent)
                {
                    auto& sc = deserializedEntity.addComponent<SpriteComponent>();
                    sc.color = spriteComponent["Color"].as<glm::vec4>();
                }
            }
    }

    void SceneSerializer::serializeEntity(Entity entity, YAML::Emitter& yaml)
    {
        yaml << YAML::BeginMap; // Entity
        yaml << YAML::Key << "Entity" << YAML::Value << "1234567890";

        {
            yaml << YAML::Key << "TagComponent" << YAML::Value;
            yaml << YAML::BeginMap; // TagComponent
            auto& tc = entity.getComponent<TagComponent>();

            yaml << YAML::Key << "Tag" << YAML::Value << tc.tag;

            yaml << YAML::EndMap; // TagComponent
        }

        {
            yaml << YAML::Key << "TransformComponent" << YAML::Value;
            yaml << YAML::BeginMap; // TransformComponent
            auto& tc = entity.getComponent<TransformComponent>();

            yaml << YAML::Key << "Translation" << YAML::Value << tc.translation;
            yaml << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
            yaml << YAML::Key << "Scale" << YAML::Value << tc.scale;

            yaml << YAML::EndMap; // TransformComponent
        }

        if (entity.hasComponent<CameraComponent>())
        {
            yaml << YAML::Key << "CameraComponent" << YAML::Value;
            yaml << YAML::BeginMap; // CameraComponent
            auto& cc = entity.getComponent<CameraComponent>();

            yaml << YAML::Key << "ProjectionType" << YAML::Value << static_cast<uint32>(cc.camera.getProjectionType());
            yaml << YAML::Key << "OrthographicSize" << YAML::Value << cc.camera.getOrthographicSize();
            yaml << YAML::Key << "OrthographicNear" << YAML::Value << cc.camera.getOrthographicNear();
            yaml << YAML::Key << "OrthographicFar" << YAML::Value << cc.camera.getOrthographicFar();
            yaml << YAML::Key << "PerspectiveFOV" << YAML::Value << cc.camera.getPerspectiveFOV();
            yaml << YAML::Key << "PerspectiveNear" << YAML::Value << cc.camera.getPerspectiveNear();
            yaml << YAML::Key << "PerspectiveFar" << YAML::Value << cc.camera.getPerspectiveFar();

            yaml << YAML::EndMap; // CameraComponent
        }

        // TODO: NativeScriptComponent

        if (entity.hasComponent<SpriteComponent>())
        {
            yaml << YAML::Key << "SpriteComponent" << YAML::Value;
            yaml << YAML::BeginMap; // SpriteComponent
            auto& sc = entity.getComponent<SpriteComponent>();

            yaml << YAML::Key << "Color" << YAML::Value << sc.color;

            yaml << YAML::EndMap; // TagCSpriteComponentomponent
        }

        yaml << YAML::EndMap; // Entity
    }

} // namespace jng
