/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/graphics_context_ogl.hpp"

#include "core/base_internal.hpp"
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
    
#ifdef JNG_DEBUG
    static void GLAPIENTRY openGlErrorHandler(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei /*length*/,
        const GLchar* message,
        const void* /*userParam*/)
    {
        // Skip
        switch (id) {
        case 131185: return; // Buffer usage hint notification
        }

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
            JNG_CORE_ERROR("OpenGL debug | {0} | Type: {1} | Severity: {2}\n  {3}",
                sourceStr, typeStr, "High", message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            JNG_CORE_ERROR("OpenGL debug | {0} | Type: {1} | Severity: {2}\n  {3}",
                sourceStr, typeStr, "Medium", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            JNG_CORE_WARN("OpenGL debug | {0} | Type: {1} | Severity: {2}\n  {3}",
                sourceStr, typeStr, "Low", message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            JNG_CORE_WARN("OpenGL debug | {0} | Type: {1} | ID: {2}\n  {3}",
                sourceStr, typeStr, id, message);
            break;
        }
    }
#endif

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
