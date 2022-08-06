/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scripting/lua_script.hpp"

#include "scene/entity.hpp"

#include <lua/lua.hpp>

namespace jng {

    LuaScript::LuaScript(Entity entity, const std::string& name) :
        m_entity{ entity },
        m_L{ LuaEngine::getLuaState() },
        m_data{ LuaEngine::getScriptData(name) }
    {
        JNG_PRINT_LUA_STACK();
        lua_getglobal(m_L, name.c_str());
        lua_newtable(m_L);
        lua_insert(m_L, -2);
        lua_pushvalue(m_L, -1);
        lua_setmetatable(m_L, -3);
        lua_setfield(m_L, -1, "__index");
        JNG_PRINT_LUA_STACK();

        lua_pushlightuserdata(m_L, &m_entity);
        lua_setfield(m_L, -2, "_entityHandle_");
    }

    void LuaScript::onCreate()
    {
        for (auto& prop : m_data.properties)
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
                lua_pushnumber(m_L, value);
                lua_setfield(m_L, -2, prop.first.c_str());
                break;
            }
        }

        if (m_data.hasOnCreate)
        {
            lua_getfield(m_L, -1, "onCreate");
            lua_pushvalue(m_L, -2); // copy script table for argument
            JNG_CORE_ASSERT(lua_isfunction(m_L, -2), "should be a function");
            JNG_CORE_ASSERT(lua_istable(m_L, -1), "should be a table");
            if (lua_pcall(m_L, 1, 0, 0))
            {
                JNG_CORE_ERROR("Lua Error: {}", lua_tostring(m_L, -1));
            }
        }
    }

    void LuaScript::onUpdate(float dt)
    {
        if (m_data.hasOnUpdate)
        {
            lua_getfield(m_L, -1, "onUpdate");
            lua_pushvalue(m_L, -2); // copy script table for argument
            lua_pushnumber(m_L, dt);
            JNG_CORE_ASSERT(lua_isfunction(m_L, -3), "should be a function");
            JNG_CORE_ASSERT(lua_istable(m_L, -2), "should be a table");
            if (lua_pcall(m_L, 2, 0, 0))
            {
                JNG_CORE_ERROR("Lua Error: {}", lua_tostring(m_L, -1));
            }
        }
    }

    Ref<LuaScript> LuaScript::create(Entity entity, const std::string& name)
    {
        return makeRef<LuaScript>(entity, name);
    }

} // namespace jng
