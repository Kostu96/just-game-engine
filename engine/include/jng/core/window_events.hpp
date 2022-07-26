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
        WindowResizeEvent(uint32 width, uint32 height) :
            m_width(width),
            m_height(height) {}

        uint32 getWidth() const { return m_width; }
        uint32 getHeight() const { return m_height; }

        EVENT_CLASS_TYPE(WindowResize)
    private:
        uint32 m_width;
        uint32 m_height;
    };

} // namespace jng
