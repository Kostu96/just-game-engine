/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scripting/lua_script.hpp"

#include <lua/lua.hpp>

namespace jng {

    LuaScript* LuaScript::create(std::filesystem::path path)
    {
        LuaScript* luaScript = new LuaScript{};

        luaScript->m_luaState = luaL_newstate();
        
        return luaScript;
    }

    void LuaScript::destroy(LuaScript* instance)
    {
        lua_close(instance->m_luaState);

        delete instance;
    }

} // namespace jng
