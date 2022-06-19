/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/engine.hpp"
#include "debug/profiling.hpp"

// TODO: force render api through cli args or debug config file
extern jng::Engine* createApp();

int main(int /*argc*/, char* /*argv*/[])
{
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

#if defined(JNG_WINDOWS) && defined(JNG_WINDOW_API_WINAPI)

#include "platform/windows/windows_base.hpp"

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
