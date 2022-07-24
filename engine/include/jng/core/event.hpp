/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <iostream>
#include <string>

namespace jng {

    // TODO: events are blocking for now, event queue in future?

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowNotFocus, WindowMove,
        GameTick, GameUpdate, GameRender,
        KeyPress, KeyRelease,
        MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
    };

    class Event
    {
    public:
        virtual ~Event() = default;

        virtual EventType getType() const = 0;
        virtual const char* getName() const = 0;
        virtual std::string to_string() const { return getName(); }
        bool isHandled() const { return m_handled; }
    protected:
        bool m_handled = false;
    private:
        friend class EventDispatcher;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& event)
    {
        return os << event.to_string();
    }

    class EventDispatcher
    {
    public:
        explicit EventDispatcher(Event& event) :
            m_event(event) {}

        template<typename T, typename F>
        bool dispatch(const F& func) {
            if (m_event.getType() == T::getStaticType()) {
                m_event.m_handled = func(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }
    private:
        Event& m_event;
    };

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; } \
                               EventType getType() const override { return getStaticType(); } \
                               const char* getName() const override { return #type; }

} // namespace jng
