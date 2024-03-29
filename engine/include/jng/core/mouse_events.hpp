/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/event.hpp"
#include "jng/platform/mouse_codes.hpp"

namespace jng {

    class MouseButtonEvent :
        public Event
    {
    public:
        Mouse::Code getMouseButton() const { return m_button; }
    protected:
        MouseButtonEvent(Mouse::Code button)
            : m_button(button) {}

        Mouse::Code m_button;
    };

    class MouseButtonPressEvent :
        public MouseButtonEvent
    {
    public:
        MouseButtonPressEvent(Mouse::Code button)
            : MouseButtonEvent(button) {}

        EVENT_CLASS_TYPE(MouseButtonPress)
    };

    class MouseButtonReleaseEvent :
        public MouseButtonEvent
    {
    public:
        MouseButtonReleaseEvent(Mouse::Code button)
            : MouseButtonEvent(button) {}

        EVENT_CLASS_TYPE(MouseButtonRelease)
    };

    class MouseMoveEvent :
        public Event
    {
    public:
        MouseMoveEvent(const float x, const float y)
            : m_mouseX(x), m_mouseY(y) {}

        float getX() const { return m_mouseX; }
        float getY() const { return m_mouseY; }

        EVENT_CLASS_TYPE(MouseMove)
    private:
        float m_mouseX, m_mouseY;
    };

    class MouseScrollEvent :
        public Event
    {
    public:
        MouseScrollEvent(const float xOffset, const float yOffset)
            : m_xOffset(xOffset), m_yOffset(yOffset) {}

        float getXOffset() const { return m_xOffset; }
        float getYOffset() const { return m_yOffset; }

        EVENT_CLASS_TYPE(MouseScroll)
    private:
        float m_xOffset, m_yOffset;
    };

} // namespace jng
