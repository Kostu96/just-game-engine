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
            m_keyCode{ keyCode } {}

        Key::Code m_keyCode;
    };

    class KeyPressEvent :
        public KeyEvent
    {
    public:
        KeyPressEvent(Key::Code keyCode, bool isRepeat) :
            KeyEvent{ keyCode },
            m_isRepeat{ isRepeat } {}

        bool isRepeat() const { return m_isRepeat; }

        EVENT_CLASS_TYPE(KeyPress)
    private:
        bool m_isRepeat;
    };

    class KeyReleaseEvent :
        public KeyEvent
    {
    public:
        explicit KeyReleaseEvent(Key::Code keyCode) :
            KeyEvent{ keyCode } {}
    
        EVENT_CLASS_TYPE(KeyRelease)
    };

} // namespace jng
