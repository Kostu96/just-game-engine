/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scripting/lua_script.hpp"

#include "utilities/file.hpp"

#include <lua/lua.hpp>

namespace jng {

    LuaScript* LuaScript::create(std::filesystem::path path)
    {
        LuaScript* luaScript = new LuaScript{};

        luaScript->m_luaState = luaL_newstate();
        lua_State* L = luaScript->m_luaState;

        std::string pathAsString = path.string();
        size_t size;
        readFile(pathAsString.c_str(), nullptr, size);
        char* scriptSource = new char[size + 1];
        readFile(pathAsString.c_str(), scriptSource, size);
        scriptSource[size] = 0;
        
        auto logFunc = [](lua_State* L) -> int {
            const char* message = lua_tostring(L, -1);
            JNG_CORE_INFO(message);

            return 0;
        };

        lua_pushcfunction(L, logFunc);
        lua_setglobal(L, "log");

        luaL_dostring(L, scriptSource);
        
        

        lua_getglobal(L, "test");
        
        if (lua_isfunction(L, -1))
        {
            if (lua_pcall(L, 0, 0, 0))
            {
                JNG_CORE_ERROR("Lua Error: {}", lua_tostring(L, -1));
            }
        }

        return luaScript;
    }

    void LuaScript::destroy(LuaScript* instance)
    {
        lua_close(instance->m_luaState);

        delete instance;
    }

} // namespace jng
