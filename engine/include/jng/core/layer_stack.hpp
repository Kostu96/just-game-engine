/*
 * Copyright (C) 2020-2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <vector>

namespace jng {

    class Layer;

    class LayerStack
    {
    public:
        using ContainerType = std::vector<Layer*>;
        using Interator = ContainerType::iterator;
        using ConstInterator = ContainerType::const_iterator;
        using ReverseInterator = ContainerType::reverse_iterator;
        using ConstReverseInterator = ContainerType::const_reverse_iterator;

        LayerStack() = default;
        ~LayerStack();

        void pushLayer(Layer* layer);
        Layer* popLayer();

        Interator begin() { return m_layers.begin(); }
        Interator end() { return m_layers.end(); }
        ConstInterator begin() const { return m_layers.begin(); }
        ConstInterator end() const { return m_layers.end(); }
        ReverseInterator rbegin() { return m_layers.rbegin(); }
        ReverseInterator rend() { return m_layers.rend(); }
        ConstReverseInterator rbegin() const { return m_layers.rbegin(); }
        ConstReverseInterator rend() const { return m_layers.rend(); }
    private:

        LayerStack(const LayerStack&) = delete;
        LayerStack& operator=(const LayerStack&) = delete;

        ContainerType m_layers;
    };

} // namespace jng
