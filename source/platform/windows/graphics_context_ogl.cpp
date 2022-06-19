/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/graphics_context_ogl.hpp"

#include "core/base_internal.hpp"
#include "platform/window.hpp"
#include "platform/windows/windows_base.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#if defined(K2D_WINDOWS)
#pragma comment(lib, "opengl32.lib")
#endif

namespace k2d {
    
#ifdef K2D_DEBUG
    static void GLAPIENTRY openGlErrorHandler(
        GLenum source,
        GLenum type,
        GLuint /*id*/,
        GLenum severity,
        GLsizei /*length*/,
        const GLchar* message,
        const void* /*userParam*/)
    {
        const char* sourceStr = "";
        switch (source) {
        case GL_DEBUG_SOURCE_API: sourceStr = "Call to the OpenGL API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "Call to a window-system API"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Compiler for a shading language"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "Application associated with OpenGL"; break;
        case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "User of this application"; break;
        case GL_DEBUG_SOURCE_OTHER: sourceStr = "Unknown"; break;
        }

        const char* typeStr = "";
        switch (type) {
        case GL_DEBUG_TYPE_ERROR: typeStr = "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "Undefined behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY: typeStr = "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "Performance"; break;
        case GL_DEBUG_TYPE_MARKER: typeStr = "Command stream annotation"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
        case GL_DEBUG_TYPE_POP_GROUP: typeStr = "User defined"; break;
        case GL_DEBUG_TYPE_OTHER: typeStr = "Unknown"; break;
        }

        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            K2D_CORE_ERROR("OpenGL debug | {0} | Type: {1} | Severity: {2}\n  {3}",
                sourceStr, typeStr, "High", message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            K2D_CORE_ERROR("OpenGL debug | {0} | Type: {1} | Severity: {2}\n  {3}",
                sourceStr, typeStr, "Medium", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            K2D_CORE_WARN("OpenGL debug | {0} | Type: {1} | Severity: {2}\n  {3}",
                sourceStr, typeStr, "Low", message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            K2D_CORE_WARN("OpenGL debug | {0} | Type: {1}\n  {2}",
                sourceStr, typeStr, message);
            break;
        }
    }
#endif

    static void* windowsGLGetProcAddress(const char* name)
    {
        void* p = reinterpret_cast<void*>(wglGetProcAddress(name));
        if (!p)
        {
            HMODULE oglLib = GetModuleHandleA("opengl32.dll");
            if (!oglLib) {
                K2D_CORE_FATAL("GetModuleHandle error! opengl32.dll is not loaded!");
                return nullptr;
            }

            p = reinterpret_cast<void*>(GetProcAddress(oglLib, name));
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
        K2D_CORE_ASSERT(pixelFormat, "Could not choose pixel format!");
        SetPixelFormat(m_deviceContext, pixelFormat, &pfd);

        m_graphicsContextHandle = wglCreateContext(m_deviceContext);
        makeCurrent();

        [[maybe_unused]] int success = gladLoadGLLoader(windowsGLGetProcAddress);
        K2D_CORE_ASSERT(success, "Could not initialize GLAD!");

        K2D_CORE_INFO("OpenGL info:");
        K2D_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        K2D_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        K2D_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

#if K2D_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(openGlErrorHandler, nullptr);
#endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    OpenGLGraphicsContext::~OpenGLGraphicsContext()
    {
        wglMakeCurrent(GetDC(m_windowHandle), nullptr);
        wglDeleteContext(m_graphicsContextHandle);
    }

    void OpenGLGraphicsContext::makeCurrent() const
    {
        wglMakeCurrent(GetDC(m_windowHandle), m_graphicsContextHandle);
    }

    void OpenGLGraphicsContext::swapBuffers() const
    {
        SwapBuffers(GetDC(m_windowHandle));
    }

} // namespace k2d
