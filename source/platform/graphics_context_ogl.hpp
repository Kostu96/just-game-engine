/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "platform/graphics_context.hpp"

#if defined(JNG_WINDOWS)
struct HGLRC__;
typedef HGLRC__* GraphicsContextHandle;
struct HWND__;
typedef HWND__* WindowHandle;
struct HDC__;
typedef HDC__* DeviceContextHandle;
#else
typedef void* GraphicsContextHandle;
typedef void* WindowHandle;
typedef void* DeviceContextHandle;
#endif

namespace jng {

    class OpenGLGraphicsContext :
        public GraphicsContext
    {
    public:
        explicit OpenGLGraphicsContext(Window& window);
        ~OpenGLGraphicsContext();

        void setVSync(bool enabled) override;

        void swapBuffers() const override;
    private:
        Window& m_window;
        WindowHandle m_windowHandle;
        DeviceContextHandle m_deviceContext;
        GraphicsContextHandle m_graphicsContextHandle;
    };

} // namespace jng
