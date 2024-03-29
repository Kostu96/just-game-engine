/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/core/base.hpp>
#include <jng/scene/entity.hpp>

class lua_State;

namespace jng {

    struct LuaScriptComponent;
    
    namespace LuaEngine {

    struct ScriptData
    {
        enum class PropertyType
        {
            Number = 0
        };

        struct Property
        {
            PropertyType type;
            void* value;
        };

        bool hasOnCreate = false;
        bool hasOnDestroy = false;
        bool hasOnUpdate = false;

        using PropertiesContainerType = std::map<std::string, Property>;
        PropertiesContainerType properties;
    };

    void init();
    void shutdown();

    std::string registerScript(const std::filesystem::path& path);
    void unregisterScripts();
    ScriptData getScriptData(const std::string& name);

    void onCreate(Entity entity, LuaScriptComponent& lsc);
    void onDestroy(Entity entity, LuaScriptComponent& lsc);
    void onUpdate(Entity entity, LuaScriptComponent& lsc, float dt);

    void printLuaStack(const char* file, int line);
    
    } // namespace LuaEngine 

} // namespace jng

#ifdef JNG_DEBUG
#define JNG_PRINT_LUA_STACK() jng::LuaEngine::printLuaStack(__FILE__, __LINE__)
#else
#define JNG_PRINT_LUA_STACK()
#endif
