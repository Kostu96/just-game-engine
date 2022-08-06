/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/scene/entity.hpp"
#include "jng/scripting/lua_engine.hpp"

namespace jng {

    class Event;
    class Scene;

    class LuaScript
    {
    public:
        LuaScript(Entity entity, const std::string& name);
        ~LuaScript() = default;

        void onCreate();
        void onUpdate(float dt);

        LuaEngine::ScriptData& getScriptData() { return m_data; }

        static Ref<LuaScript> create(Entity entity, const std::string& name);
    protected:
        template<typename T>
        T& getComponent() { return m_entity.getComponent<T>(); }

        Entity createEntity(const std::string& name) { return m_entity.getScene()->createEntity(name); }
        void destroyEntity(Entity entity) { return m_entity.getScene()->destroyEntity(entity); }
    private:
        Entity m_entity;
        std::string m_name;
        lua_State* m_L;
        LuaEngine::ScriptData m_data;

        friend class Scene;
    };

} // namespace jng
