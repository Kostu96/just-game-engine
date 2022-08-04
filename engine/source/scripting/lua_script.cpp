/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scripting/lua_script.hpp"

#include <lua/lua.hpp>

namespace jng {

    LuaScript::LuaScript(std::filesystem::path path) :
        m_name{ path.stem().string() },
        m_luaState{ luaL_newstate() }
    {
        lua_State* L = m_luaState;
        luaL_openlibs(L);

#pragma region internalDef
        {
            auto logFunc = [](lua_State* L) -> int {
                const char* message = lua_tostring(L, -1);
                JNG_CORE_INFO(message);

                return 0;
            };
            lua_pushcfunction(L, logFunc);
            lua_setglobal(L, "log");
        }
#pragma endregion

        luaL_dofile(L, path.string().c_str());

        JNG_CORE_TRACE("Reflecting on {} script:", m_name);
        auto reflect = fmt::format(
            R"===(
symbols = {{}}
values = {{}}
i = 1
for key, value in pairs({0}) do
    symbols[i] = key
    values[i] = value
    i = i + 1
end
)===", m_name);

        if (luaL_dostring(L, reflect.c_str()))
        {
            JNG_CORE_ERROR("Lua Error: {}", lua_tostring(L, -1));
        }

        lua_getglobal(L, "symbols");
        lua_getglobal(L, "values");
        JNG_CORE_ASSERT(lua_istable(L, -2), "\"symbols\" must be a Lua table!");
        JNG_CORE_ASSERT(lua_istable(L, -1), "\"values\" must be a Lua table!");
        uint32 size = (uint32)lua_rawlen(L, -1);

        for (uint32 i = 1; i <= size; i++)
        {
            int type = lua_rawgeti(L, -1, i);
            lua_pop(L, 1);
            lua_rawgeti(L, -2, i);
            const char* str = lua_tostring(L, -1);
            switch (type)
            {
            case LUA_TFUNCTION: JNG_CORE_TRACE("  {}.{} - Function", m_name, str); break;
            case LUA_TNUMBER:   JNG_CORE_TRACE("  {}.{} - Number", m_name, str); break;
            case LUA_TSTRING:   JNG_CORE_TRACE("  {}.{} - String", m_name, str); break;
            default:            JNG_CORE_TRACE("  {}.{} - Other", m_name, str); break;
}
            lua_pop(L, 1);
        }
    }

    LuaScript::~LuaScript()
    {
        lua_close(m_luaState);
    }

    Ref<LuaScript> LuaScript::create(std::filesystem::path path)
    {
        return makeRef<LuaScript>(path);
    }

} // namespace jng
