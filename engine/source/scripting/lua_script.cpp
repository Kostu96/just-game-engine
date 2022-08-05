/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scripting/lua_script.hpp"

#include "scene/components.hpp"
#include "scene/entity.hpp"
#include "platform/input.hpp"
#include "platform/key_codes.hpp"

#include <lua/lua.hpp>

namespace jng {

    LuaScript::LuaScript(std::filesystem::path path) :
        m_name{ path.stem().string() },
        m_luaState{ luaL_newstate() }
    {
        lua_State* L = m_luaState;
        luaL_openlibs(L);

#pragma region internalDef
        // definitions
        enum Components
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

        lua_newtable(L);
        lua_pushinteger(L, Tag);
        lua_setfield(L, -2, "Tag");
        lua_pushinteger(L, Transform);
        lua_setfield(L, -2, "Transform");
        lua_pushinteger(L, Camera);
        lua_setfield(L, -2, "Camera");
        lua_pushinteger(L, SpriteRenderer);
        lua_setfield(L, -2, "SpriteRenderer");
        lua_pushinteger(L, CircleRenderer);
        lua_setfield(L, -2, "CircleRenderer");
        lua_pushinteger(L, BoxCollider2D);
        lua_setfield(L, -2, "BoxCollider2D");
        lua_pushinteger(L, CircleCollider2D);
        lua_setfield(L, -2, "CircleCollider2D");
        lua_pushinteger(L, Rigidbody2D);
        lua_setfield(L, -2, "Rigidbody2D");
        lua_setglobal(L, "Components");

        // global functions
        {
            auto logFunc = [](lua_State* L) -> int {
                const char* message = lua_tostring(L, -1);
                JNG_CORE_INFO(message);

                return 0;
            };
            lua_pushcfunction(L, logFunc);
            lua_setglobal(L, "log");

            auto input_isKeyPressed = [](lua_State* L) -> int {
                int64 key = luaL_checkinteger(L, 1);
                bool isPressed = Input::isKeyPressed((Key::Code)key);
                lua_pushboolean(L, isPressed);
                return 1;
            };

            lua_newtable(L);
            lua_newtable(L);
            lua_pushnumber(L, Key::A);
            lua_setfield(L, -2, "A");
            lua_pushnumber(L, Key::D);
            lua_setfield(L, -2, "D");
            lua_setfield(L, -2, "Key");
            lua_pushcfunction(L, input_isKeyPressed);
            lua_setfield(L, -2, "isKeyPressed");
            lua_setglobal(L, "Input");
        }

        // script base
        {
            auto luaScript_new = [](lua_State* L) -> int {
                JNG_CORE_ASSERT(lua_istable(L, 1), "luaScript_new 1st parameter is not a table!");

                lua_newtable(L);                      // LuaScript table - stack: -1 LuaScript table, -2 self arg
                lua_insert(L, -2);                    // exchange LuaScript table with self on the stack - stack: -1 self arg, -2 LuaScript table
                lua_pushvalue(L, -1);             // copy self to the top of the stack - stack: -1 self copy, -2 self arg, -3 LuaScript table
                lua_setmetatable(L, -3);      // set new LuaScript table metateble to self; pops the self copy - stack: -1 self arg, -2 LuaScript table
                lua_setfield(L, -1, "__index"); // self.__index = self; pops self - stack: -1 LuaScript table
                return 1;
            };

            struct LuaRigidbody2DComponent
            {
                Rigidbody2DComponent* handle;
            };

            auto luaScript_getComponent = [](lua_State* L) -> int {
                JNG_CORE_ASSERT(lua_istable(L, 1), "luaScript_getComponent 1st parameter is not a table!");
                JNG_CORE_ASSERT(lua_isnumber(L, 2), "luaScript_getComponent 2nd parameter is not a number!");

                lua_getfield(L, 1, "_entityHandle_");
                Entity* entityHandle = reinterpret_cast<Entity*>(lua_touserdata(L, -1));
                lua_pop(L, 1);

                int64 type = luaL_checkinteger(L, 2);
                switch (type)
                {
                case Tag:
                    break;
                case Transform:
                    break;
                case Camera:
                    break;
                case SpriteRenderer:
                    break;
                case CircleRenderer:
                    break;
                case BoxCollider2D:
                    break;
                case CircleCollider2D:
                    break;
                case Rigidbody2D:
                    LuaRigidbody2DComponent* luaRBC = reinterpret_cast<LuaRigidbody2DComponent*>(lua_newuserdata(L, sizeof(LuaRigidbody2DComponent)));
                    luaL_getmetatable(L, "JNG.LuaRigidbody2DComponent");
                    lua_setmetatable(L, -2);
                    luaRBC->handle = &entityHandle->getComponent<Rigidbody2DComponent>();
                    break;
                }

                return 1;
            };

            auto luaRigidbody2dComponent_setVelocity = [](lua_State* L) -> int {
                LuaRigidbody2DComponent* luaRBC = reinterpret_cast<LuaRigidbody2DComponent*>(luaL_checkudata(L, 1, "JNG.LuaRigidbody2DComponent"));
                JNG_CORE_ASSERT(luaRBC, "luaRigidbody2dComponent_setVelocity 1st parameter is not a LuaRigidbody2DComponent!");
                JNG_CORE_ASSERT(lua_isnumber(L, 2), "luaRigidbody2dComponent_setVelocity 2nd parameter is not a number!");
                JNG_CORE_ASSERT(lua_isnumber(L, 3), "luaRigidbody2dComponent_setVelocity 3rd parameter is not a number!");
                float x = (float)lua_tonumber(L, 2);
                float y = (float)lua_tonumber(L, 3);
                luaRBC->handle->setLinearVelocity({ x, y });
                return 0;
            };

            luaL_newmetatable(L, "JNG.LuaRigidbody2DComponent");
            lua_pushstring(L, "__index");
            lua_pushvalue(L, -2); // pushes the metatable
            lua_settable(L, -3);  // metatable.__index = metatable
            lua_pushcfunction(L, luaRigidbody2dComponent_setVelocity);
            lua_setfield(L, -2, "setVelocity");

            lua_newtable(L);
            lua_pushlightuserdata(L, &m_entity);
            lua_setfield(L, -2, "_entityHandle_");
            lua_pushcfunction(L, luaScript_new);
            lua_setfield(L, -2, "new");
            lua_pushcfunction(L, luaScript_getComponent);
            lua_setfield(L, -2, "getComponent");
            lua_setglobal(L, "LuaScript");
        }
#pragma endregion

        if (luaL_dofile(L, path.string().c_str()))
        {
            JNG_CORE_ERROR("Lua Error: {}", lua_tostring(L, -1));
        }

        JNG_CORE_TRACE("Reflecting on {} script:", m_name);

        lua_pushnil(L); // push nil as first key because lua_next needs something to pop
        while (lua_next(L, -2) != 0)
        {
            const char* symbol = lua_type(L, -2) == LUA_TSTRING ? lua_tostring(L, -2) : "!not_a_string!";
            int type = lua_type(L, -1);

            JNG_CORE_TRACE("{}.{} - {}", m_name, symbol, lua_typename(L, type));

            switch (type)
            {
            case LUA_TFUNCTION:
                if (strcmp(symbol, "onCreate") == 0) m_hasOnCreateFunction = true;
                else if (strcmp(symbol, "onUpdate") == 0) m_hasOnUpdateFunction = true;
                
                break;
            }

            // pop 'value', leave 'key' for lua_next to pop
            lua_pop(L, 1);
        }
    }

    LuaScript::~LuaScript()
    {
        lua_close(m_luaState);
    }

    void LuaScript::onCreate()
    {
        lua_State* L = m_luaState;
        
        if (m_hasOnCreateFunction)
        {
            lua_getfield(L, -1, "onCreate");
            lua_pushvalue(L, -2); // copy script table for argument
            JNG_CORE_ASSERT(lua_isfunction(L, -2), "should be a function");
            JNG_CORE_ASSERT(lua_istable(L, -1), "should be a table");
            if (lua_pcall(L, 1, 0, 0))
            {
                JNG_CORE_ERROR("Lua Error: {}", lua_tostring(L, -1));
            }
        }
    }

    void LuaScript::onUpdate(float dt)
    {
        lua_State* L = m_luaState;

        if (m_hasOnUpdateFunction)
        {
            lua_getfield(L, -1, "onUpdate");
            lua_pushvalue(L, -2); // copy script table for argument
            lua_pushnumber(L, dt);
            JNG_CORE_ASSERT(lua_isfunction(L, -3), "should be a function");
            JNG_CORE_ASSERT(lua_istable(L, -2), "should be a table");
            if (lua_pcall(L, 2, 0, 0))
            {
                JNG_CORE_ERROR("Lua Error: {}", lua_tostring(L, -1));
            }
        }
    }

    Ref<LuaScript> LuaScript::create(std::filesystem::path path)
    {
        return makeRef<LuaScript>(path);
    }

} // namespace jng
