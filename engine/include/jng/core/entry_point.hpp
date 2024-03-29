/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/engine.hpp"
#include "jng/debug/log.hpp"
#include "jng/debug/profiling.hpp"

extern jng::Engine* createApp();

int main(int argc, char* argv[])
{
    using namespace jng;

    JNG_PROFILE_BEGIN_SESSION("profiling_init.json");
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

#if defined(_WIN32)

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
