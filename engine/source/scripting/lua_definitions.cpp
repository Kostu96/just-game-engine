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

#define JNG_LUA_CCALL_ENTRY(numParams, numReturns) \
    constexpr static const int NUM_PARAMS = numParams; \
    constexpr static const int NUM_RETURNS = numReturns; \
    [[maybe_unused]] int previousTop = lua_gettop(L)

#define JNG_LUA_CALL_EXIT() \
    JNG_CORE_ASSERT(lua_gettop(L) == previousTop - NUM_PARAMS + NUM_RETURNS, "Lua stack has incorrect size!"); \
    return NUM_RETURNS

namespace jng {

    namespace LuaScript {

        int create(lua_State* L)
        {
            JNG_LUA_CCALL_ENTRY(1, 1);

            JNG_CORE_ASSERT(lua_istable(L, 1), "LuaScript::create - 1st parameter is not a table!");

            lua_newtable(L);
            lua_insert(L, -2);
            lua_pushvalue(L, -1);
            lua_setmetatable(L, -3);
            lua_setfield(L, -1, "__index");

            JNG_LUA_CALL_EXIT();
        }

        int getComponent(lua_State* L)
        {
            JNG_LUA_CCALL_ENTRY(2, 1);

            JNG_CORE_ASSERT(lua_istable(L, 1), "LuaScript::getComponent - 1st parameter is not a table!");
            JNG_CORE_ASSERT(lua_isnumber(L, 2), "LuaScript::getComponent - 2nd parameter is not a number!");

            lua_getfield(L, 1, "_entityHandle_");
            entt::entity entityHandle = (entt::entity)(u64)lua_touserdata(L, -1);
            lua_pop(L, 1);
            lua_getfield(L, 1, "_sceneHandle_");
            Scene* sceneHandle = (Scene*)lua_touserdata(L, -1);
            lua_pop(L, 1);

            Entity entity{ entityHandle, *sceneHandle };

            s64 type = luaL_checkinteger(L, 2);
            lua_pop(L, 2); // pop parameters

            switch (type)
            {
            case LuaComponent::Tag:
                break;
            case LuaComponent::Transform:
                break;
            case LuaComponent::Camera:
                break;
            case LuaComponent::SpriteRenderer:
                break;
            case LuaComponent::CircleRenderer:
                break;
            case LuaComponent::BoxCollider2D:
                break;
            case LuaComponent::CircleCollider2D:
                break;
            case LuaComponent::Rigidbody2D:
                LuaComponent::LuaRigidbody2DComponent* rbc =
                    reinterpret_cast<LuaComponent::LuaRigidbody2DComponent*>(lua_newuserdata(L, sizeof(LuaComponent::LuaRigidbody2DComponent)));
                rbc->handle = &entity.getComponent<Rigidbody2DComponent>();
                luaL_getmetatable(L, LuaComponent::LuaRigidbody2DComponent::METATABLE_NAME);
                lua_setmetatable(L, -2);
                break;
            }

            JNG_LUA_CALL_EXIT();
        }

    } // namespace LuaScript

    namespace LuaGlobal {

        int log(lua_State* L)
        {
            JNG_LUA_CCALL_ENTRY(1, 0);

            JNG_CORE_ASSERT(lua_isstring(L, 1), "Global::log - 1st parameter is not a string!");

            const char* message = lua_tostring(L, -1);
            JNG_CORE_INFO(message);

            JNG_LUA_CALL_EXIT();
        }

    } // namespace LuaGlobal

    namespace LuaComponent {

        int LuaRigidbody2DComponent::setLinearVelocity(lua_State* L)
        {
            JNG_LUA_CCALL_ENTRY(3, 0);

            LuaRigidbody2DComponent* rbc = reinterpret_cast<LuaRigidbody2DComponent*>(luaL_checkudata(L, 1, LuaRigidbody2DComponent::METATABLE_NAME));
            JNG_CORE_ASSERT(rbc, "Rigidbody2dComponent::setLinearVelocity - 1st parameter is not a LuaRigidbody2DComponent!");
            JNG_CORE_ASSERT(lua_isnumber(L, 2), "Rigidbody2dComponent::setLinearVelocity - 2nd parameter is not a number!");
            JNG_CORE_ASSERT(lua_isnumber(L, 3), "Rigidbody2dComponent::setLinearVelocity - 3rd parameter is not a number!");
            
            float x = (float)lua_tonumber(L, 2);
            float y = (float)lua_tonumber(L, 3);
            rbc->handle->setLinearVelocity({ x, y });
            
            lua_pop(L, 3); // pop parameters

            JNG_LUA_CALL_EXIT();
        }

    } // namespace LuaComponent

    namespace LuaInput {

        int isKeyPressed(lua_State* L)
        {
            JNG_LUA_CCALL_ENTRY(1, 1);

            JNG_CORE_ASSERT(lua_isnumber(L, 1), "Input::isKeyPressed - 1st parameter is not a number!");

            Key::Code key = static_cast<u16>(luaL_checkinteger(L, 1));
            lua_pop(L, 1); // pop parameter

            bool isPressed = jng::Input::isKeyPressed(key);
            lua_pushboolean(L, isPressed);
            
            JNG_LUA_CALL_EXIT();
        }

    } // namespace LuaInput

} // namespace jng
