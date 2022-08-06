/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scripting/lua_script.hpp"

#include "scene/entity.hpp"

#include <lua/lua.hpp>

namespace jng {

    LuaScript::LuaScript(std::filesystem::path path) :
        m_name{ path.stem().string() }
    {
        /*lua_pushlightuserdata(L, &m_entity);
        lua_setfield(L, -2, "_entityHandle_");*/
    }

    void LuaScript::onCreate()
    {
        if (m_hasOnCreateFunction)
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
        if (m_hasOnUpdateFunction)
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

    Ref<LuaScript> LuaScript::create(std::filesystem::path path)
    {
        return makeRef<LuaScript>(path);
    }

} // namespace jng
