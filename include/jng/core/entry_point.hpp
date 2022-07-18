/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/engine.hpp"
#include "jng/debug/log.hpp"
#include "jng/debug/profiling.hpp"
#include "jng/renderer/renderer_api.hpp"

extern jng::Engine* createApp();

int main(int argc, char* argv[])
{
    using namespace jng;

    JNG_PROFILE_BEGIN_SESSION("profiling_init.json");

    // TODO: do proper render API choosing
    RendererBackend backend = RendererBackend::OpenGL;
    for (int i = 1; i < argc; ++i)
        if (std::strcmp(argv[i], "--backend") == 0) {
            if (i + 1 >= argc) {
                JNG_CORE_ERROR("Bad command line arguments");
                return -1;
            }

            if (std::strcmp(argv[i + 1], "d3d") == 0)    backend = RendererBackend::Direct3D;
            else if (std::strcmp(argv[i + 1], "ogl") == 0) backend = RendererBackend::OpenGL;
            else if (std::strcmp(argv[i + 1], "vlk") == 0) backend = RendererBackend::Vulkan;
            else {
                JNG_CORE_ERROR("Bad command line arguments");
                return -1;
            }
        }
    RendererAPI::init(backend);

    auto app = createApp();

    JNG_PROFILE_END_SESSION();

    JNG_PROFILE_BEGIN_SESSION("profiling_run.json");
    app->run();
    JNG_PROFILE_END_SESSION();

    JNG_PROFILE_BEGIN_SESSION("profiling_shutdown.json");
    delete app;
    JNG_PROFILE_END_SESSION();

    return 0;
}

#if defined(JNG_WINDOWS)

#define WINAPI __stdcall
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
typedef wchar_t* LPWSTR;
typedef char* LPSTR;

#if defined(UNICODE)

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    return main(__argc, __argv);
}

#else

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    return main(__argc, __argv);
}

#endif

#endif
