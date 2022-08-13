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

    struct LuaScene
    {
        Scene* sceneHandle;

        static int createEntity(lua_State* L);

        static constexpr const char* METATABLE_NAME = "JNG.Scene";
    };

    struct LuaEntity
    {
        void* entityHandle;
        Scene* sceneHandle;

        static int addComponent(lua_State* L);
        static int getComponent(lua_State* L);

        static int setPosition(lua_State* L);
        static int getPosition(lua_State* L);
        static int getScale(lua_State* L);
        static int move(lua_State* L);
        static int scale(lua_State* L);

        static constexpr const char* METATABLE_NAME = "JNG.Entity";
    };

    namespace LuaGlobal {

        int log(lua_State* L);

    } // namespace LuaGlobal

#pragma region LuaComponent
        
        enum class LuaComponentID : s64
        {
            Camera,
            SpriteRenderer,
            CircleRenderer,
            BoxCollider2D,
            CircleCollider2D,
            Rigidbody2D
        };

        struct LuaCameraComponent
        {
            CameraComponent* handle;

            static constexpr const char* METATABLE_NAME = "JNG.LuaCameraComponent";
        };

        struct LuaSpriteRendererComponent
        {
            SpriteRendererComponent* handle;

            static constexpr const char* METATABLE_NAME = "JNG.SpriteRendererComponent";
        };

        struct LuaCircleRendererComponent
        {
            CircleRendererComponent* handle;

            static constexpr const char* METATABLE_NAME = "JNG.LuaCircleRendererComponent";
        };

        struct LuaBoxCollider2DComponent
        {
            BoxCollider2DComponent* handle;

            static constexpr const char* METATABLE_NAME = "JNG.LuaBoxCollider2DComponent";
        };

        struct LuaCircleCollider2DComponent
        {
            CircleCollider2DComponent* handle;

            static constexpr const char* METATABLE_NAME = "JNG.LuaCircleCollider2DComponent";
        };

        struct LuaRigidbody2DComponent
        {
            Rigidbody2DComponent* handle;

            static int setLinearVelocity(lua_State* L);

            static constexpr const char* METATABLE_NAME = "JNG.Rigidbody2DComponenet";
        };

#pragma endregion

    namespace LuaInput {

        int isKeyPressed(lua_State* L);

    } // namespace LuaInput

} // namespace jng
