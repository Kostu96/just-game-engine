/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "lua_definitions.hpp"

#include "platform/input.hpp"
#include "platform/key_codes.hpp"
#include "scene/entity.hpp"
#include "scripting/lua_engine.hpp"

#include <lua/lua.hpp>

namespace jng::Lua {

    namespace Script {

        int jng::Lua::Script::create(lua_State* L)
        {
            JNG_CORE_ASSERT(lua_istable(L, 1), "luaScript_new 1st parameter is not a table!");

            JNG_PRINT_LUA_STACK();
            lua_newtable(L);                      // new script
            lua_insert(L, -2);                    // exchange new script with self on the stack
            lua_pushvalue(L, -1);             // copy self to the top of the stack
            lua_setmetatable(L, -3);      // set new script metateble to self; pops the stack
            lua_setfield(L, -1, "__index"); // self.__index = self; pops the stack 
            JNG_PRINT_LUA_STACK();

            return 1;
        }

        int jng::Lua::Script::getComponent(lua_State* L)
        {
            JNG_CORE_ASSERT(lua_istable(L, 1), "luaScript_getComponent 1st parameter is not a table!");
            JNG_CORE_ASSERT(lua_isnumber(L, 2), "luaScript_getComponent 2nd parameter is not a number!");

            lua_getfield(L, 1, "_entityHandle_");
            entt::entity entityHandle = (entt::entity)(uint64)lua_touserdata(L, -1);
            lua_pop(L, 1);
            lua_getfield(L, 1, "_sceneHandle_");
            Scene* sceneHandle = (Scene*)lua_touserdata(L, -1);
            lua_pop(L, 1);

            Entity entity{ entityHandle, *sceneHandle };

            int64 type = luaL_checkinteger(L, 2);
            switch (type)
            {
            case Component::Tag:
                break;
            case Component::Transform:
                break;
            case Component::Camera:
                break;
            case Component::SpriteRenderer:
                break;
            case Component::CircleRenderer:
                break;
            case Component::BoxCollider2D:
                break;
            case Component::CircleCollider2D:
                break;
            case Component::Rigidbody2D:
                Component::Rigidbody2DComponent* rbc = reinterpret_cast<Component::Rigidbody2DComponent*>(lua_newuserdata(L, sizeof(Component::Rigidbody2DComponent)));
                rbc->handle = &entity.getComponent<Rigidbody2DComponent>();
                luaL_getmetatable(L, Component::Rigidbody2DComponent::METATABLE_NAME);
                lua_setmetatable(L, -2);
                break;
            }

            return 1;
        }

    } // namespace Script

    namespace Global {

        int log(lua_State* L)
        {
            const char* message = lua_tostring(L, -1);
            JNG_CORE_INFO(message);

            return 0;
        }

    } // namespace Global

    namespace Component {

        int Rigidbody2DComponent::setLinearVelocity(lua_State* L)
        {
            Rigidbody2DComponent* rbc = reinterpret_cast<Rigidbody2DComponent*>(luaL_checkudata(L, 1, Rigidbody2DComponent::METATABLE_NAME));

            JNG_CORE_ASSERT(rbc, "Rigidbody2dComponent::setLinearVelocity 1st parameter is not a LuaRigidbody2DComponent!");
            JNG_CORE_ASSERT(lua_isnumber(L, 2), "Rigidbody2dComponent::setLinearVelocity 2nd parameter is not a number!");
            JNG_CORE_ASSERT(lua_isnumber(L, 3), "Rigidbody2dComponent::setLinearVelocity 3rd parameter is not a number!");
            
            float x = (float)lua_tonumber(L, 2);
            float y = (float)lua_tonumber(L, 3);
            rbc->handle->setLinearVelocity({ x, y });
            
            return 0;
        }

    } // namespace Component

    namespace Input {

        int isKeyPressed(lua_State* L)
        {
            Key::Code key = static_cast<uint16>(luaL_checkinteger(L, 1));
            bool isPressed = jng::Input::isKeyPressed(key);
            lua_pushboolean(L, isPressed);
            
            return 1;
        }

    } // namespace Input

} // namespace jng::Lua
