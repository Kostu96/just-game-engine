/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "platform/graphics_context.hpp"

// TODO: temp
struct HGLRC__;
typedef HGLRC__* HGLRC;
struct HWND__;
typedef HWND__* HWND;
struct HDC__;
typedef HDC__* HDC;

namespace jng {

    class OpenGLGraphicsContext :
        public GraphicsContext
    {
    public:
        explicit OpenGLGraphicsContext(Window& window);
        ~OpenGLGraphicsContext();

        void makeCurrent() const override;
        void swapBuffers() const override;
    private:
        // TODO: temp
        Window& m_window;
        HWND m_windowHandle;
        HDC m_deviceContext;
        HGLRC m_graphicsContextHandle;
    };

} // namespace jng
