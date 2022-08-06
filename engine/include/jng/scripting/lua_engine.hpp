/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/core/base.hpp>
#include <jng/scene/entity.hpp>

#include <map>

class lua_State;

namespace jng::LuaEngine {
    
    struct ScriptData
    {
        enum class PropertyType
        {
            Number
        };

        struct Property
        {
            PropertyType type;
            void* value;
        };

        bool hasOnCreate = false;
        bool hasOnUpdate = false;

        std::map<std::string, Property> properties;
    };

    void init();
    void shutdown();
    
    lua_State* getLuaState();

    std::string registerScript(const std::filesystem::path& path);
    ScriptData getScriptData(const std::string& name);
    void registerScriptInstance(const std::string& name, Entity entity);


    void printLuaStack(const char* file, int line);

} // namespace jng::LuaEngine

#ifdef JNG_DEBUG
#define JNG_PRINT_LUA_STACK() jng::LuaEngine::printLuaStack(__FILE__, __LINE__)
#else
#define JNG_PRINT_LUA_STACK()
#endif
