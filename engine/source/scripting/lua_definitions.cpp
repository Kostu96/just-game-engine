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

#define JNG_LUA_CALL_ENTRY(numParams, numReturns) \
    constexpr static const int NUM_PARAMS = numParams; \
    constexpr static const int NUM_RETURNS = numReturns; \
    [[maybe_unused]] int previousTop = lua_gettop(L)

#define JNG_LUA_CALL_EXIT() \
    JNG_CORE_ASSERT(lua_gettop(L) == previousTop - NUM_PARAMS + NUM_RETURNS, "Lua stack has incorrect size!"); \
    return NUM_RETURNS

namespace jng {

#pragma region LuaScene

    int LuaScene::createEntity(lua_State* L)
    {
        JNG_LUA_CALL_ENTRY(1, 1);

        LuaScene* luaScene = reinterpret_cast<LuaScene*>(luaL_checkudata(L, 1, LuaScene::METATABLE_NAME));
        JNG_CORE_ASSERT(luaScene, "LuaScene::addComponent - 1st parameter is not a LuaScene!");
        JNG_CORE_ASSERT(lua_isstring(L, 2), "LuaScene::createEntity - 1st parameter is not a string!");

        const char* name = lua_tostring(L, 2);
        Entity entity = luaScene->sceneHandle->createEntity(name);
        lua_pop(L, 1);

        LuaEntity* luaEntity = reinterpret_cast<LuaEntity*>(lua_newuserdata(L, sizeof(LuaEntity)));
        luaEntity->entityHandle = entity;
        luaEntity->sceneHandle = luaScene->sceneHandle;
        luaL_getmetatable(L, LuaEntity::METATABLE_NAME);
        lua_setmetatable(L, -2);

        JNG_LUA_CALL_EXIT();
    }

#pragma endregion

#pragma region LuaEntity

    int LuaEntity::addComponent(lua_State* L)
    {
        JNG_LUA_CALL_ENTRY(2, 1);

        LuaEntity* luaEntity = reinterpret_cast<LuaEntity*>(luaL_checkudata(L, 1, LuaEntity::METATABLE_NAME));
        JNG_CORE_ASSERT(luaEntity, "LuaEntity::addComponent - 1st parameter is not a LuaEntity!");
        JNG_CORE_ASSERT(lua_isnumber(L, 2), "LuaEntity::addComponent - 2nd parameter is not a number!");

        entt::entity entityHandle = (entt::entity)(u64)luaEntity->entityHandle;
        Entity entity{ entityHandle, *luaEntity->sceneHandle };

        LuaComponentID type = static_cast<LuaComponentID>(luaL_checkinteger(L, 2));
        lua_pop(L, 2); // pop parameters

#define ADD_COMPONENT_CASE(type, ...) \
case LuaComponentID::type: { \
    Lua##type##Component* comp = reinterpret_cast<Lua##type##Component*>(lua_newuserdata(L, sizeof(Lua##type##Component))); \
    comp->handle = &entity.addComponent<type##Component>(__VA_ARGS__); \
    luaL_getmetatable(L, Lua##type##Component::METATABLE_NAME); \
    lua_setmetatable(L, -2); \
} break

        switch (type)
        {
            ADD_COMPONENT_CASE(Camera);
            ADD_COMPONENT_CASE(SpriteRenderer);
            ADD_COMPONENT_CASE(CircleRenderer);
            ADD_COMPONENT_CASE(BoxCollider2D);
            ADD_COMPONENT_CASE(CircleCollider2D);
            ADD_COMPONENT_CASE(Rigidbody2D);
        }
            
#undef ADD_COMPONENT_CASE

        JNG_LUA_CALL_EXIT();
    }

    int LuaEntity::getComponent(lua_State* L)
    {
        JNG_LUA_CALL_ENTRY(2, 1);

        LuaEntity* luaEntity = reinterpret_cast<LuaEntity*>(luaL_checkudata(L, 1, LuaEntity::METATABLE_NAME));
        JNG_CORE_ASSERT(luaEntity, "LuaEntity::getComponent - 1st parameter is not a LuaEntity!");
        JNG_CORE_ASSERT(lua_isnumber(L, 2), "LuaEntity::getComponent - 2nd parameter is not a number!");

        entt::entity entityHandle = (entt::entity)(u64)luaEntity->entityHandle;
        Entity entity{ entityHandle, *luaEntity->sceneHandle };

        LuaComponentID type = static_cast<LuaComponentID>(luaL_checkinteger(L, 2));
        lua_pop(L, 2); // pop parameters

#define GET_COMPONENT_CASE(type) \
    case LuaComponentID::type: { \
        Lua##type##Component* comp = reinterpret_cast<Lua##type##Component*>(lua_newuserdata(L, sizeof(Lua##type##Component))); \
        comp->handle = &entity.getComponent<type##Component>(); \
        luaL_getmetatable(L, Lua##type##Component::METATABLE_NAME); \
        lua_setmetatable(L, -2); \
    } break

        switch (type)
        {
            GET_COMPONENT_CASE(Camera);
            GET_COMPONENT_CASE(SpriteRenderer);
            GET_COMPONENT_CASE(CircleRenderer);
            GET_COMPONENT_CASE(BoxCollider2D);
            GET_COMPONENT_CASE(CircleCollider2D);
            GET_COMPONENT_CASE(Rigidbody2D);
        }

#undef GET_COMPONENT_CASE

        JNG_LUA_CALL_EXIT();
    }

    int LuaEntity::setPosition(lua_State* L)
    {
        JNG_LUA_CALL_ENTRY(4, 0);

        LuaEntity* luaEntity = reinterpret_cast<LuaEntity*>(luaL_checkudata(L, 1, LuaEntity::METATABLE_NAME));
        JNG_CORE_ASSERT(luaEntity, "LuaEntity::scale - 1st parameter is not a LuaEntity!");
        JNG_CORE_ASSERT(lua_isnumber(L, 2), "LuaScript::move - 2nd parameter is not a number!");
        JNG_CORE_ASSERT(lua_isnumber(L, 3), "LuaScript::move - 3rd parameter is not a number!");
        JNG_CORE_ASSERT(lua_isnumber(L, 4), "LuaScript::move - 3rd parameter is not a number!");

        entt::entity entityHandle = (entt::entity)(u64)luaEntity->entityHandle;
        Entity entity{ entityHandle, *luaEntity->sceneHandle };
        auto& tc = entity.getComponent<TransformComponent>();

        tc.translation.x = (float)lua_tonumber(L, 2);
        tc.translation.y = (float)lua_tonumber(L, 3);
        tc.translation.z = (float)lua_tonumber(L, 4);

        lua_pop(L, 4);
        JNG_LUA_CALL_EXIT();
    }

    int LuaEntity::getPosition(lua_State* L)
    {
        JNG_LUA_CALL_ENTRY(1, 3);

        LuaEntity* luaEntity = reinterpret_cast<LuaEntity*>(luaL_checkudata(L, 1, LuaEntity::METATABLE_NAME));
        JNG_CORE_ASSERT(luaEntity, "LuaEntity::getPosition - 1st parameter is not a LuaEntity!");

        entt::entity entityHandle = (entt::entity)(u64)luaEntity->entityHandle;
        Entity entity{ entityHandle, *luaEntity->sceneHandle };
        auto& tc = entity.getComponent<TransformComponent>();
        lua_pop(L, 1);

        lua_pushnumber(L, tc.translation.x);
        lua_pushnumber(L, tc.translation.y);
        lua_pushnumber(L, tc.translation.z);

        JNG_LUA_CALL_EXIT();
    }

    int LuaEntity::getScale(lua_State* L)
    {
        JNG_LUA_CALL_ENTRY(1, 3);

        LuaEntity* luaEntity = reinterpret_cast<LuaEntity*>(luaL_checkudata(L, 1, LuaEntity::METATABLE_NAME));
        JNG_CORE_ASSERT(luaEntity, "LuaEntity::getScale - 1st parameter is not a LuaEntity!");

        entt::entity entityHandle = (entt::entity)(u64)luaEntity->entityHandle;
        Entity entity{ entityHandle, *luaEntity->sceneHandle };
        auto& tc = entity.getComponent<TransformComponent>();
        lua_pop(L, 1);

        lua_pushnumber(L, tc.scale.x);
        lua_pushnumber(L, tc.scale.y);
        lua_pushnumber(L, tc.scale.z);

        JNG_LUA_CALL_EXIT();
    }

    int LuaEntity::move(lua_State* L)
    {
        JNG_LUA_CALL_ENTRY(4, 0);

        LuaEntity* luaEntity = reinterpret_cast<LuaEntity*>(luaL_checkudata(L, 1, LuaEntity::METATABLE_NAME));
        JNG_CORE_ASSERT(luaEntity, "LuaEntity::scale - 1st parameter is not a LuaEntity!");
        JNG_CORE_ASSERT(lua_isnumber(L, 2), "LuaScript::move - 2nd parameter is not a number!");
        JNG_CORE_ASSERT(lua_isnumber(L, 3), "LuaScript::move - 3rd parameter is not a number!");
        JNG_CORE_ASSERT(lua_isnumber(L, 4), "LuaScript::move - 3rd parameter is not a number!");

        entt::entity entityHandle = (entt::entity)(u64)luaEntity->entityHandle;
        Entity entity{ entityHandle, *luaEntity->sceneHandle };
        auto& tc = entity.getComponent<TransformComponent>();

        tc.translation.x += (float)lua_tonumber(L, 2);
        tc.translation.y += (float)lua_tonumber(L, 3);
        tc.translation.z += (float)lua_tonumber(L, 4);

        lua_pop(L, 4);
        JNG_LUA_CALL_EXIT();
    }

    int LuaEntity::scale(lua_State* L)
    {
        JNG_LUA_CALL_ENTRY(4, 0);

        LuaEntity* luaEntity = reinterpret_cast<LuaEntity*>(luaL_checkudata(L, 1, LuaEntity::METATABLE_NAME));
        JNG_CORE_ASSERT(luaEntity, "LuaEntity::scale - 1st parameter is not a LuaEntity!");
        JNG_CORE_ASSERT(lua_isnumber(L, 2), "LuaEntity::scale - 2nd parameter is not a number!");
        JNG_CORE_ASSERT(lua_isnumber(L, 3), "LuaEntity::scale - 2nd parameter is not a number!");
        JNG_CORE_ASSERT(lua_isnumber(L, 4), "LuaEntity::scale - 2nd parameter is not a number!");

        entt::entity entityHandle = (entt::entity)(u64)luaEntity->entityHandle;
        Entity entity{ entityHandle, *luaEntity->sceneHandle };
        auto& tc = entity.getComponent<TransformComponent>();

        tc.scale.x *= (float)lua_tonumber(L, 2);
        tc.scale.y *= (float)lua_tonumber(L, 3);
        tc.scale.z *= (float)lua_tonumber(L, 4);

        lua_pop(L, 4);
        JNG_LUA_CALL_EXIT();
    }

#pragma endregion

    namespace LuaGlobal {

        int log(lua_State* L)
        {
            JNG_LUA_CALL_ENTRY(1, 0);

            JNG_CORE_ASSERT(lua_isstring(L, 1), "Global::log - 1st parameter is not a string!");

            const char* message = lua_tostring(L, -1);
            JNG_CORE_INFO(message);

            JNG_LUA_CALL_EXIT();
        }

    } // namespace LuaGlobal

#pragma region LuaComponent

    int LuaRigidbody2DComponent::setLinearVelocity(lua_State* L)
    {
        JNG_LUA_CALL_ENTRY(3, 0);

        LuaRigidbody2DComponent* rbc = reinterpret_cast<LuaRigidbody2DComponent*>(luaL_checkudata(L, 1, LuaRigidbody2DComponent::METATABLE_NAME));
        JNG_CORE_ASSERT(rbc, "Rigidbody2dComponent::setLinearVelocity - 1st parameter is not a LuaRigidbody2DComponent!");
        JNG_CORE_ASSERT(lua_isnumber(L, 2), "Rigidbody2dComponent::setLinearVelocity - 2nd parameter is not a number!");
        JNG_CORE_ASSERT(lua_isnumber(L, 3), "Rigidbody2dComponent::setLinearVelocity - 3rd parameter is not a number!");
            
        float x = (float)lua_tonumber(L, 2);
        float y = (float)lua_tonumber(L, 3);
        rbc->handle->setLinearVelocity({ x, y });
            
        lua_pop(L, 3);
        JNG_LUA_CALL_EXIT();
    }

#pragma endregion

    namespace LuaInput {

        int isKeyPressed(lua_State* L)
        {
            JNG_LUA_CALL_ENTRY(1, 1);

            JNG_CORE_ASSERT(lua_isnumber(L, 1), "Input::isKeyPressed - 1st parameter is not a number!");

            Key::Code key = static_cast<u16>(luaL_checkinteger(L, 1));
            lua_pop(L, 1); // pop parameter

            bool isPressed = jng::Input::isKeyPressed(key);
            lua_pushboolean(L, isPressed);
            
            JNG_LUA_CALL_EXIT();
        }

    } // namespace LuaInput

} // namespace jng
