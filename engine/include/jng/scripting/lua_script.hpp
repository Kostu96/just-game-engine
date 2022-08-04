/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/scene/entity.hpp>

#include <filesystem>

struct lua_State;

namespace jng {

    class Event;
    class Scene;

    class LuaScript
    {
    public:
        explicit LuaScript(std::filesystem::path path);
        ~LuaScript();

        const std::string& getName() const { return m_name; }

        static Ref<LuaScript> create(std::filesystem::path path);
    protected:
        template<typename T>
        T& getComponent() { return m_entity.getComponent<T>(); }

        Entity createEntity(const std::string& name) { return m_entity.getScene()->createEntity(name); }
        void destroyEntity(Entity entity) { return m_entity.getScene()->destroyEntity(entity); }
    private:
        std::string m_name;
        lua_State* m_luaState = nullptr;
        Entity m_entity;

        friend class Scene;
    };

} // namespace jng
