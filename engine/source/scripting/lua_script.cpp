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
        m_name{ name },
        m_L{ LuaEngine::getLuaState() },
        m_data{ LuaEngine::getScriptData(m_name) }
    {
        LuaEngine::registerScriptInstance(m_name, entity);
    }

    void LuaScript::onCreate()
    {
        JNG_CORE_ASSERT(lua_gettop(m_L) == 0, "Lua stack should be empty!");

        std::string instanceName = m_name + std::to_string(m_entity.getGUID());
        lua_getglobal(m_L, "_scriptInstances_");
        lua_getfield(m_L, -1, instanceName.c_str());

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

        lua_pop(m_L, 2);

        JNG_CORE_ASSERT(lua_gettop(m_L) == 0, "Lua stack should be empty!");
    }

    void LuaScript::onUpdate(float dt)
    {
        JNG_CORE_ASSERT(lua_gettop(m_L) == 0, "Lua stack should be empty!");

        std::string instanceName = m_name + std::to_string(m_entity.getGUID());
        lua_getglobal(m_L, "_scriptInstances_");
        lua_getfield(m_L, -1, instanceName.c_str());

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

        lua_pop(m_L, 2);

        JNG_CORE_ASSERT(lua_gettop(m_L) == 0, "Lua stack should be empty!");
    }

    Ref<LuaScript> LuaScript::create(Entity entity, const std::string& name)
    {
        return makeRef<LuaScript>(entity, name);
    }

} // namespace jng
