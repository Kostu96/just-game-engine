/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/debug/log.hpp"

#ifdef JNG_DEBUG

#include <glad/gl.h>

namespace jng {

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

} // namespace jng

#endif
