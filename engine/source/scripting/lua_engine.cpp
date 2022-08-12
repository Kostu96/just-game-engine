/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scripting/lua_engine.hpp"

#include "platform/key_codes.hpp"
#include "scene/components.hpp"
#include "scripting/lua_definitions.hpp"

#include <lua/lua.hpp>

namespace jng::LuaEngine {

    struct LuaEngineData
    {
        lua_State* L = nullptr;

        std::unordered_map<std::string, ScriptData> scripts;
    };

    static LuaEngineData s_data;

    static void loadJNGDefinitions()
    {
#pragma region LuaScript
        lua_newtable(s_data.L);

        lua_pushcfunction(s_data.L, LuaScript::create);
        lua_setfield(s_data.L, -2, "create");
        lua_pushcfunction(s_data.L, LuaScript::getComponent);
        lua_setfield(s_data.L, -2, "getComponent");

        lua_setglobal(s_data.L, "LuaScript");
#pragma endregion

#pragma region LuaGlobal
        lua_pushcfunction(s_data.L, LuaGlobal::log);
        lua_setglobal(s_data.L, "log");
#pragma endregion

#pragma region LuaComponent
        lua_newtable(s_data.L);

        lua_pushinteger(s_data.L, LuaComponent::Tag);
        lua_setfield(s_data.L, -2, "Tag");
        lua_pushinteger(s_data.L, LuaComponent::Transform);
        lua_setfield(s_data.L, -2, "Transform");
        lua_pushinteger(s_data.L, LuaComponent::Camera);
        lua_setfield(s_data.L, -2, "Camera");
        lua_pushinteger(s_data.L, LuaComponent::SpriteRenderer);
        lua_setfield(s_data.L, -2, "SpriteRenderer");
        lua_pushinteger(s_data.L, LuaComponent::CircleRenderer);
        lua_setfield(s_data.L, -2, "CircleRenderer");
        lua_pushinteger(s_data.L, LuaComponent::BoxCollider2D);
        lua_setfield(s_data.L, -2, "BoxCollider2D");
        lua_pushinteger(s_data.L, LuaComponent::CircleCollider2D);
        lua_setfield(s_data.L, -2, "CircleCollider2D");
        lua_pushinteger(s_data.L, LuaComponent::Rigidbody2D);
        lua_setfield(s_data.L, -2, "Rigidbody2D");

        lua_setglobal(s_data.L, "Component");

        luaL_newmetatable(s_data.L, LuaComponent::LuaRigidbody2DComponent::METATABLE_NAME);

        lua_pushstring(s_data.L, "__index");
        lua_pushvalue(s_data.L, -2); // pushes the metatable
        lua_settable(s_data.L, -3);  // metatable.__index = metatable

        lua_pushcfunction(s_data.L, LuaComponent::LuaRigidbody2DComponent::setLinearVelocity);
        lua_setfield(s_data.L, -2, "setLinearVelocity");

        lua_pop(s_data.L, 1);
#pragma endregion

#pragma region LuaInput
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

        lua_pushcfunction(s_data.L, LuaInput::isKeyPressed);
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

        lua_newtable(s_data.L);
        lua_setglobal(s_data.L, "_scriptInstances_");
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
                    union { double value; void* any; };
                    value = lua_tonumber(s_data.L, -1);
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

    void onCreate(Entity entity, LuaScriptComponent& lsc)
    {
        JNG_CORE_ASSERT(s_data.scripts.find(lsc.name) != s_data.scripts.end(), "Script is not registered!");
        JNG_CORE_ASSERT(lua_gettop(s_data.L) == 0, "Lua stack should be empty!");

        std::string instanceName = lsc.name + std::to_string(entity.getGUID());
        JNG_CORE_TRACE("Registering script instance: {}", instanceName);

        lua_getglobal(s_data.L, "_scriptInstances_");

        lua_getglobal(s_data.L, lsc.name.c_str());
        lua_newtable(s_data.L);
        lua_insert(s_data.L, -2);
        lua_pushvalue(s_data.L, -1);
        lua_setmetatable(s_data.L, -3);
        lua_setfield(s_data.L, -1, "__index");

        lua_pushlightuserdata(s_data.L, entity);
        lua_setfield(s_data.L, -2, "_entityHandle_");
        lua_pushlightuserdata(s_data.L, entity.getScene());
        lua_setfield(s_data.L, -2, "_sceneHandle_");

        for (auto& prop : lsc.data.properties)
        {
            switch (prop.second.type)
            {
            case LuaEngine::ScriptData::PropertyType::Number:
                union
                {
                    double value;
                    void* any;
                };
                any = prop.second.value;
                lua_pushnumber(s_data.L, value);
                lua_setfield(s_data.L, -2, prop.first.c_str());
                break;
            }
        }

        lua_setfield(s_data.L, -2, instanceName.c_str());

        lua_getfield(s_data.L, -1, instanceName.c_str());
        if (lsc.data.hasOnCreate)
        {
            lua_getfield(s_data.L, -1, "onCreate");
            lua_pushvalue(s_data.L, -2); // copy script table for argument
            JNG_CORE_ASSERT(lua_isfunction(s_data.L, -2), "should be a function");
            JNG_CORE_ASSERT(lua_istable(s_data.L, -1), "should be a table");
            if (lua_pcall(s_data.L, 1, 0, 0))
            {
                JNG_CORE_ERROR("Lua Error: {}", lua_tostring(s_data.L, -1));
            }
        }

        lua_pop(s_data.L, 2);

        JNG_CORE_ASSERT(lua_gettop(s_data.L) == 0, "Lua stack should be empty!");
    }

    void onUpdate(Entity entity, LuaScriptComponent& lsc, float dt)
    {
        JNG_CORE_ASSERT(s_data.scripts.find(lsc.name) != s_data.scripts.end(), "Script is not registered!");
        JNG_CORE_ASSERT(lua_gettop(s_data.L) == 0, "Lua stack should be empty!");

        std::string instanceName = lsc.name + std::to_string(entity.getGUID());
        lua_getglobal(s_data.L, "_scriptInstances_");
        lua_getfield(s_data.L, -1, instanceName.c_str());

        if (lsc.data.hasOnUpdate)
        {
            lua_getfield(s_data.L, -1, "onUpdate");
            lua_pushvalue(s_data.L, -2); // copy script table for argument
            lua_pushnumber(s_data.L, dt);
            JNG_CORE_ASSERT(lua_isfunction(s_data.L, -3), "should be a function");
            JNG_CORE_ASSERT(lua_istable(s_data.L, -2), "should be a table");
            if (lua_pcall(s_data.L, 2, 0, 0))
            {
                JNG_CORE_ERROR("Lua Error: {}", lua_tostring(s_data.L, -1));
            }
        }

        lua_pop(s_data.L, 2);

        JNG_CORE_ASSERT(lua_gettop(s_data.L) == 0, "Lua stack should be empty!");
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
