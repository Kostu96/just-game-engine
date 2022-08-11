/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/graphics_context.hpp"

#include "core/base_internal.hpp"
#include "platform/window.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace jng {

#ifdef JNG_DEBUG

    static void GLAPIENTRY openGlErrorHandler(GLenum source, GLenum type, GLenum id, GLuint severity, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/)
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

    GraphicsContext::GraphicsContext(Window& window) :
        m_window{ window }
    {
        glfwMakeContextCurrent(m_window.getNativeWindowHandle());

        [[maybe_unused]] int success = gladLoadGL(glfwGetProcAddress);
        JNG_CORE_ASSERT(success, "Could not load GL extensions!");

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

        glLineWidth(3.f);

        setVSync(true);
    }

    void GraphicsContext::setVSync(bool enabled)
    {
        m_isVSync = enabled;
        glfwSwapInterval(enabled ? 1 : 0);
    }

    void GraphicsContext::swapBuffers() const
    {
        glfwSwapBuffers(m_window.getNativeWindowHandle());
    }

    std::array<GraphicsContext::Hint, 4> GraphicsContext::getContextCreationHints()
    {
        std::array<Hint, 4> hints{};
        hints[0] = { GLFW_CONTEXT_VERSION_MAJOR, 4 };
        hints[1] = { GLFW_CONTEXT_VERSION_MINOR, 6 };
        hints[2] = { GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE };
#ifdef JNG_DEBUG
        hints[3] = { GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE };
#else
        hints[3] = { GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE };
#endif

        return hints;
    }

} // namespace jng
