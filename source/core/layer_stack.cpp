/*
 * Copyright (C) 2020-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "core/layer_stack.hpp"

#include "core/base_internal.hpp"
#include "core/layer.hpp"

namespace jng {

    LayerStack::~LayerStack()
    {
        JNG_PROFILE_FUNCTION();

        for (auto layer = m_layers.rbegin(); layer != m_layers.rend(); ++layer)
        {
            (*layer)->onDetach();
            delete *layer;
        }
    }

    void LayerStack::pushLayer(Layer* layer)
    {
        JNG_PROFILE_FUNCTION();

        m_layers.emplace_back(layer);
        layer->onAttach();
    }

    Layer* LayerStack::popLayer()
    {
        JNG_PROFILE_FUNCTION();

        Layer* layer = m_layers.back();
        m_layers.pop_back();
        layer->onDetach();
        return layer;
    }

} // namespace jng
