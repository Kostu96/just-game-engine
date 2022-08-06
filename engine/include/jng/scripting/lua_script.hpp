/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/scene/entity.hpp>

struct lua_State;

namespace jng {

    class Event;
    class Scene;

    class LuaScript
    {
    public:
        explicit LuaScript(std::filesystem::path path);
        ~LuaScript() = default;

        const std::string& getName() const { return m_name; }

        void onCreate();
        void onUpdate(float dt);

        static Ref<LuaScript> create(std::filesystem::path path);
    protected:
        template<typename T>
        T& getComponent() { return m_entity.getComponent<T>(); }

        Entity createEntity(const std::string& name) { return m_entity.getScene()->createEntity(name); }
        void destroyEntity(Entity entity) { return m_entity.getScene()->destroyEntity(entity); }
    private:
        std::string m_name;
        lua_State* m_L = nullptr;
        bool m_hasOnCreateFunction = false;
        bool m_hasOnUpdateFunction = false;

        Entity m_entity;

        friend class Scene;
    };

} // namespace jng
