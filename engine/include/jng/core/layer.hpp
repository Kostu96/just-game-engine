/*
 * Copyright (C) 2020-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

namespace jng {

    class Event;
    class Context;
    class LayerStack;

    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate(float /*dt*/) {}
        virtual void onImGuiUpdate() {}
        virtual void onEvent(Event& /*event*/) {}
    };

} // namespace jng
