/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/event.hpp"
#include "jng/platform/key_codes.hpp"

namespace jng {

    class KeyEvent :
        public Event
    {
    public:
        Key::Code getKeyCode() const { return m_keyCode; }
    protected:
        explicit KeyEvent(Key::Code keyCode) :
            m_keyCode(keyCode) {}

        Key::Code m_keyCode;
    };

    class KeyPressEvent :
        public KeyEvent
    {
    public:
        KeyPressEvent(Key::Code keyCode, unsigned int repeatCount) :
            KeyEvent(keyCode),
            m_repeatCount(repeatCount) {}

        unsigned int getRepeatCount() const { return m_repeatCount; }

        EVENT_CLASS_TYPE(KeyPress)
    private:
        unsigned int m_repeatCount;
    };

    class KeyReleaseEvent :
        public KeyEvent
    {
    public:
        explicit KeyReleaseEvent(Key::Code keyCode) :
            KeyEvent(keyCode) {}
    
        EVENT_CLASS_TYPE(KeyRelease)
    };

} // namespace jng
