/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"
#include "jng/scene/components.hpp"

class lua_State;

namespace jng::Lua {

    namespace Script {

        int create(lua_State* L);
        int getComponent(lua_State* L);

    } // namespace Script

    namespace Global {

        int log(lua_State* L);

    } // namespace Global

    namespace Component {
        
        enum
        {
            Tag,
            Transform,
            Camera,
            SpriteRenderer,
            CircleRenderer,
            BoxCollider2D,
            CircleCollider2D,
            Rigidbody2D
        };

        struct TagComponent              { jng::TagComponent*              handle; };
        //struct TransformComponent        { jng::TransformComponent*        handle; };
        struct CameraComponent           { jng::CameraComponent*           handle; };
        struct SpriteRendererComponent   { jng::SpriteRendererComponent*   handle; };
        struct CircleRendererComponent   { jng::CircleRendererComponent*   handle; };
        struct BoxCollider2DComponent    { jng::BoxCollider2DComponent*    handle; };
        struct CircleCollider2DComponent { jng::CircleCollider2DComponent* handle; };

        struct Rigidbody2DComponent
        {
            jng::Rigidbody2DComponent* handle;

            static int setLinearVelocity(lua_State* L);

            static constexpr const char* METATABLE_NAME = "JNG.Rigidbody2DComponenet";
        };

    } // namespace Component

    namespace Input {

        int isKeyPressed(lua_State* L);

    } // namespace Input

} // namespace jng::Lua
