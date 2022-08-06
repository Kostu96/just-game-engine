/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/scene/scene.hpp>

namespace YAML {

    class Emitter;

} // namespace YAML

namespace jng {

    class SceneSerializer
    {
    public:
        explicit SceneSerializer(const Ref<Scene>& scene) : m_scene{ scene } {}

        void serialize(const std::filesystem::path& path);
        void deserialize(const std::filesystem::path& path);
    private:
        void serializeEntity(Entity entity, YAML::Emitter& yaml);

        Ref<Scene> m_scene;
    };

} // namespace jng
