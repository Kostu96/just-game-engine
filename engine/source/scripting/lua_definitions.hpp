/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"
#include "jng/scene/components.hpp"

class lua_State;

namespace jng {

    namespace LuaScript {

        struct LuaEntity
        {
            void* handle;

            static constexpr const char* METATABLE_NAME = "JNG.Entity";
        };

        int create(lua_State* L);
        int getComponent(lua_State* L);

        int createEntity(lua_State* L);

    } // namespace LuaScript

    namespace LuaGlobal {

        int log(lua_State* L);

    } // namespace LuaGlobal

    namespace LuaComponent {
        
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

        struct LuaTagComponent              { TagComponent*              handle; };
        struct LuaTransformComponent        { TransformComponent*        handle; };
        struct LuaCameraComponent           { CameraComponent*           handle; };
        struct LuaSpriteRendererComponent   { SpriteRendererComponent*   handle; };
        struct LuaCircleRendererComponent   { CircleRendererComponent*   handle; };
        struct LuaBoxCollider2DComponent    { BoxCollider2DComponent*    handle; };
        struct LuaCircleCollider2DComponent { CircleCollider2DComponent* handle; };

        struct LuaRigidbody2DComponent
        {
            Rigidbody2DComponent* handle;

            static int setLinearVelocity(lua_State* L);

            static constexpr const char* METATABLE_NAME = "JNG.Rigidbody2DComponenet";
        };

    } // namespace LuaComponent

    namespace LuaInput {

        int isKeyPressed(lua_State* L);

    } // namespace LuaInput

} // namespace jng
