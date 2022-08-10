/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/graphics_context_ogl.hpp"

#include "core/base_internal.hpp"
#include "debug/ogl_error_callback.hpp"
#include "platform/window.hpp"
#include "platform/windows/windows_base.hpp"

#include <glad/gl.h>
#include <glad/wgl.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#if defined(JNG_WINDOWS)
#pragma comment(lib, "opengl32.lib")
#endif

namespace jng {

    static GLADapiproc windowsGLGetProcAddress(const char* name)
    {
        GLADapiproc p = reinterpret_cast<GLADapiproc>(wglGetProcAddress(name));
        if (!p)
        {
            HMODULE oglLib = GetModuleHandleA("opengl32.dll");
            if (!oglLib) {
                JNG_CORE_FATAL("GetModuleHandle error! opengl32.dll is not loaded!");
                return nullptr;
            }

            p = reinterpret_cast<GLADapiproc>(GetProcAddress(oglLib, name));
        }

        return p;
    }

    OpenGLGraphicsContext::OpenGLGraphicsContext(Window& window) :
        m_window{ window },
        m_windowHandle{ glfwGetWin32Window(m_window.getNativeWindowHandle()) },
        m_deviceContext{ GetDC(m_windowHandle) }
    {
        PIXELFORMATDESCRIPTOR pfd{};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;

        int pixelFormat = ChoosePixelFormat(m_deviceContext, &pfd);
        JNG_CORE_ASSERT(pixelFormat, "Could not choose pixel format!");
        SetPixelFormat(m_deviceContext, pixelFormat, &pfd);

        m_graphicsContextHandle = wglCreateContext(m_deviceContext);
        wglMakeCurrent(m_deviceContext, m_graphicsContextHandle);
        
        [[maybe_unused]] int success = gladLoadGL(windowsGLGetProcAddress);
        JNG_CORE_ASSERT(success, "Could not load GL extensions!");
        success = gladLoadWGL(m_deviceContext, windowsGLGetProcAddress);
        JNG_CORE_ASSERT(success, "Could not load WGL extensions!");

        JNG_CORE_INFO("OpenGL info:");
        JNG_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
        JNG_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        JNG_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
        JNG_CORE_INFO("  GLSL Version: {0}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

#if JNG_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(openGlErrorHandler, nullptr);
#endif

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);

        // TODO: temporary:
        glLineWidth(3.f);
    }

    OpenGLGraphicsContext::~OpenGLGraphicsContext()
    {
        wglMakeCurrent(GetDC(m_windowHandle), nullptr);
        wglDeleteContext(m_graphicsContextHandle);
    }

    void OpenGLGraphicsContext::setVSync(bool enabled)
    {
        wglSwapIntervalEXT(enabled ? 1 : 0);
    }

    void OpenGLGraphicsContext::swapBuffers() const
    {
        SwapBuffers(GetDC(m_windowHandle));
    }

} // namespace jng
