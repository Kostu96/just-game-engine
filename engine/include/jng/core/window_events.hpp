/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/event.hpp"
#include "jng/core/base.hpp"

namespace jng {

    class WindowCloseEvent :
        public Event
    {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
    };

    class WindowResizeEvent :
        public Event
    {
    public:
        WindowResizeEvent(u32 width, u32 height) :
            m_width(width),
            m_height(height) {}

        u32 getWidth() const { return m_width; }
        u32 getHeight() const { return m_height; }

        EVENT_CLASS_TYPE(WindowResize)
    private:
        u32 m_width;
        u32 m_height;
    };

} // namespace jng
