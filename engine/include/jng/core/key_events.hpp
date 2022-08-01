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
        int getKeyCode() const { return m_keyCode; }
    protected:
        explicit KeyEvent(Key::KeyCode keyCode) :
            m_keyCode(keyCode) {}

        Key::KeyCode m_keyCode;
    };

    class KeyPressEvent :
        public KeyEvent
    {
    public:
        KeyPressEvent(Key::KeyCode keyCode, unsigned int repeatCount) :
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
        explicit KeyReleaseEvent(Key::KeyCode keyCode) :
            KeyEvent(keyCode) {}
    
        EVENT_CLASS_TYPE(KeyRelease)
    };

} // namespace jng
