/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/renderer/camera.hpp"

#include <glm/glm.hpp>
#include <type_traits>

namespace jng {

    struct CameraComponent
    {
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        Camera camera{ glm::mat4{1.f} };
    };

    class NativeScript;

    struct NativeScriptComponent
    {
        NativeScriptComponent() = default;
        NativeScriptComponent(const NativeScriptComponent&) = default;

        NativeScript* instance = nullptr;

        NativeScript* (*createScript)();
        void (*destroyScript)(NativeScript*&);

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
    };

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
        glm::mat4 transform{ 1.f };
    };

} // namespace jng
