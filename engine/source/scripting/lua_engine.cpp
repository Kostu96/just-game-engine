/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scripting/lua_engine.hpp"

#include "platform/key_codes.hpp"
#include "scripting/lua_definitions.hpp"

#include <lua/lua.hpp>

#include <unordered_map>

namespace jng::LuaEngine {

    struct LuaEngineData
    {
        lua_State* L = nullptr;

        std::unordered_map<std::string, ScriptData> scripts;
    };

    static LuaEngineData s_data;

    static void loadJNGDefinitions()
    {
#pragma region Script
        lua_newtable(s_data.L);

        lua_pushcfunction(s_data.L, Lua::Script::create);
        lua_setfield(s_data.L, -2, "create");
        lua_pushcfunction(s_data.L, Lua::Script::getComponent);
        lua_setfield(s_data.L, -2, "getComponent");

        lua_setglobal(s_data.L, "LuaScript");
#pragma endregion

#pragma region Global
        lua_pushcfunction(s_data.L, Lua::Global::log);
        lua_setglobal(s_data.L, "log");
#pragma endregion

#pragma region Component
        lua_newtable(s_data.L); // Component

        lua_pushinteger(s_data.L, Lua::Component::Tag);
        lua_setfield(s_data.L, -2, "Tag");
        lua_pushinteger(s_data.L, Lua::Component::Transform);
        lua_setfield(s_data.L, -2, "Transform");
        lua_pushinteger(s_data.L, Lua::Component::Camera);
        lua_setfield(s_data.L, -2, "Camera");
        lua_pushinteger(s_data.L, Lua::Component::SpriteRenderer);
        lua_setfield(s_data.L, -2, "SpriteRenderer");
        lua_pushinteger(s_data.L, Lua::Component::CircleRenderer);
        lua_setfield(s_data.L, -2, "CircleRenderer");
        lua_pushinteger(s_data.L, Lua::Component::BoxCollider2D);
        lua_setfield(s_data.L, -2, "BoxCollider2D");
        lua_pushinteger(s_data.L, Lua::Component::CircleCollider2D);
        lua_setfield(s_data.L, -2, "CircleCollider2D");
        lua_pushinteger(s_data.L, Lua::Component::Rigidbody2D);
        lua_setfield(s_data.L, -2, "Rigidbody2D");

        lua_setglobal(s_data.L, "Component");

        luaL_newmetatable(s_data.L, Lua::Component::Rigidbody2DComponent::METATABLE_NAME);

        lua_pushstring(s_data.L, "__index");
        lua_pushvalue(s_data.L, -2); // pushes the metatable
        lua_settable(s_data.L, -3);  // metatable.__index = metatable

        lua_pushcfunction(s_data.L, Lua::Component::Rigidbody2DComponent::setLinearVelocity);
        lua_setfield(s_data.L, -2, "setLinearVelocity");

        lua_pop(s_data.L, 1);
#pragma endregion

#pragma region Input
        lua_newtable(s_data.L); // Input

        lua_newtable(s_data.L); // Key

        lua_pushnumber(s_data.L, Key::Space);
        lua_setfield(s_data.L, -2, "Space");
        lua_pushnumber(s_data.L, Key::A);
        lua_setfield(s_data.L, -2, "A");
        lua_pushnumber(s_data.L, Key::D);
        lua_setfield(s_data.L, -2, "D");
        lua_pushnumber(s_data.L, Key::S);
        lua_setfield(s_data.L, -2, "S");
        lua_pushnumber(s_data.L, Key::W);
        lua_setfield(s_data.L, -2, "W");

        lua_setfield(s_data.L, -2, "Key");

        lua_pushcfunction(s_data.L, Lua::Input::isKeyPressed);
        lua_setfield(s_data.L, -2, "isKeyPressed");

        lua_setglobal(s_data.L, "Input");
#pragma endregion
    }

    void init()
    {
        s_data.L = luaL_newstate();
        JNG_CORE_ASSERT(lua_gettop(s_data.L) == 0, "Lua stack should be empty!");

        luaL_openlibs(s_data.L);
        JNG_CORE_ASSERT(lua_gettop(s_data.L) == 0, "Lua stack should be empty!");

        loadJNGDefinitions();
        JNG_CORE_ASSERT(lua_gettop(s_data.L) == 0, "Lua stack should be empty!");
    }

    void shutdown()
    {
        lua_close(s_data.L);
    }

    lua_State* getLuaState()
    {
        return s_data.L;
    }

    std::string registerScript(const std::filesystem::path& path)
    {
        std::string name = path.stem().string();
        auto [it, inserted] = s_data.scripts.emplace(name, ScriptData{});

        if (inserted)
        {
            if (luaL_dofile(s_data.L, path.string().c_str()))
            {
                JNG_CORE_ERROR("Lua Error: {}", lua_tostring(s_data.L, -1));
            }

            JNG_CORE_TRACE("Reflecting on {} script:", name);

            lua_pushnil(s_data.L); // push nil as first key because lua_next needs something to pop
            while (lua_next(s_data.L, -2) != 0)
            {
                const char* symbol = lua_type(s_data.L, -2) == LUA_TSTRING ? lua_tostring(s_data.L, -2) : "!not_a_string!";
                int type = lua_type(s_data.L, -1);

                JNG_CORE_TRACE("{}.{} - {}", name, symbol, lua_typename(s_data.L, type));

                switch (type)
                {
                case LUA_TFUNCTION:
                    if (strcmp(symbol, "onCreate") == 0) it->second.hasOnCreate = true;
                    else if (strcmp(symbol, "onUpdate") == 0) it->second.hasOnUpdate = true;

                    break;
                case LUA_TNUMBER:
                    union
                    {
                        double initialValue;
                        void* any;
                    };
                    initialValue = lua_tonumber(s_data.L, -1);
                    it->second.properties.emplace(symbol, ScriptData::Property{ ScriptData::PropertyType::Number, any });
                    break;
                }

                // pop 'value', leave 'key' for lua_next to pop
                lua_pop(s_data.L, 1);
            }

            lua_pop(s_data.L, 1);
        }
        JNG_CORE_ASSERT(lua_gettop(s_data.L) == 0, "Lua stack should be empty!");

        return name;
    }

    ScriptData getScriptData(const std::string& name)
    {
        auto data = s_data.scripts.find(name);
        JNG_CORE_ASSERT(data != s_data.scripts.end(), "Lua script is not registered in LuaEngine!");

        return data->second;
    }

    void printLuaStack(const char* file, int line)
    {
        std::filesystem::path path{ file };
        std::stringstream ss;
        int top = lua_gettop(s_data.L);
        for (int i = 1; i <= top; i++)
        {
            int type = lua_type(s_data.L, i);
            switch (type)
            {
            case LUA_TSTRING: {
                const char* str = lua_tostring(s_data.L, i);
                ss << "STRING: " << str << " ";
            }   break;
            case LUA_TBOOLEAN: {
                bool value = lua_toboolean(s_data.L, i);
                ss << "BOOL: " << value << " ";
            }   break;
            case LUA_TNUMBER: {
                double value = lua_tonumber(s_data.L, i);
                ss << "NUMBER: " << value << " ";
            }   break;
            default: {
                const char* str = lua_typename(s_data.L, type);
                ss << str << " ";
            }   break;
            }
        }

        JNG_CORE_TRACE("{}:{} Lua Stack - {}", path.filename().string(), line, ss.str());
    }

} // namespace jng::LuaEngine
