/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/graphics_context_ogl.hpp"

#include "core/base_internal.hpp"
#include "platform/window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#include <GL/glx.h>

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

    OpenGLGraphicsContext::OpenGLGraphicsContext(Window& window) :
        m_window{ window },
        m_windowHandle{ glfwGetX11Window(m_window.getNativeWindowHandle()) },
        m_deviceContext{ GetDC(m_windowHandle) }
    {
        int visual_attribs[]{
            GLX_RENDER_TYPE, GLX_RGBA_BIT,
            GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
            GLX_DOUBLE_BUFFER, true,
            GLX_RED_SIZE, 1,
            GLX_GREEN_SIZE, 1,
            GLX_BLUE_SIZE, 1,
            None
        };

        int num_fbc = 0;
        GLXFBConfig* fbc = glXChooseFBConfig(m_display, DefaultScreen(m_display), visual_attribs, &num_fbc);

        //[[maybe_unused]] int success = gladLoadGLLoader(windowsGLGetProcAddress);
        JNG_CORE_ASSERT(success, "Could not initialize GLAD!");

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
    }

    OpenGLGraphicsContext::~OpenGLGraphicsContext()
    {
        
    }

    void OpenGLGraphicsContext::makeCurrent() const
    {
        
    }

    void OpenGLGraphicsContext::swapBuffers() const
    {
        
    }

} // namespace jng
