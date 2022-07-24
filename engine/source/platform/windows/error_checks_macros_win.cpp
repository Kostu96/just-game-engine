/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/windows/error_checks_macros_win.hpp"

#if defined(JNG_DEBUG)

#include "debug/log.hpp"

#include <d3d11.h>
#include <dxerr.h>
#include <dxgidebug.h>

#pragma comment(lib, "dxguid.lib")

namespace jng {

    void wnd_check_error(HRESULT hr, const char* file, int line)
    {
        char* message = nullptr;
        DWORD messageSize = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            hr,
            0x0409, // en-US language id
            reinterpret_cast<LPSTR>(&message),
            0,
            nullptr
        );
        JNG_CORE_ERROR("WinAPI Error nr {0}\n{3}\nFile: {1}:{2}\n", hr, file, line, (messageSize > 0 ? message : "Unknown"));
        LocalFree(message);
    }

    void gfx_check_error(HRESULT hr, const char* file, int line)
    {
        char description[512];
        DXGetErrorDescriptionA(hr, description, 512);
        JNG_CORE_ERROR("D3D Error nr {0} - {1}\n{4}\nFile: {2}:{3}\n", hr, DXGetErrorStringA(hr), file, line, (description[0] == '\0' ? "Empty" : description));
    }

    void gfx_check_error_device_removed(HRESULT hr, const wrl::ComPtr<ID3D11Device>& device, const char* file, int line)
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
            hr = device->GetDeviceRemovedReason();

        gfx_check_error(hr, file, line);
    }

    void DXGIInfoManager::checkMessages(const char* file, int line)
    {
        const auto end = m_DXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
        for (auto i = m_next; i < end; i++)
        {
            SIZE_T messageLength{};
            m_DXGIInfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, nullptr, &messageLength);
            auto bytes = std::make_unique<byte[]>(messageLength);
            auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
            m_DXGIInfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, pMessage, &messageLength);
            switch (pMessage->Severity)
            {
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
                JNG_CORE_ERROR("{0}\nFile: {1}:{2}\n", pMessage->pDescription, file, line);
                break;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
                JNG_CORE_WARN("{0}\nFile: {1}:{2}\n", pMessage->pDescription, file, line);
                break;
            }
        }

        m_next = end;
    }

    DXGIInfoManager::DXGIInfoManager() :
        m_next(0)
    {
        typedef HRESULT(WINAPI* DXGIGetDebugInterfaceFunc)(REFIID, void**);

        HMODULE hModDxgiDebug = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        [[maybe_unused]] DWORD err = GetLastError();
        if (!hModDxgiDebug) {
            JNG_CORE_FATAL("LoadLibrary error! dxgidebug.dll not found!");
            return;
        }

        auto DXGIGetDebugInterface =
            reinterpret_cast<DXGIGetDebugInterfaceFunc>(
                GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"));
        if (!DXGIGetDebugInterface)
            JNG_CORE_FATAL("GetProcAddress error! DXGIGetDebugInterface definition not found!");

        [[ maybe_unused ]] HRESULT hr = DXGIGetDebugInterface(__uuidof(IDXGIInfoQueue), &m_DXGIInfoQueue);
        JNG_D3D_CHECK_HR(hr);
    }

    DXGIInfoManager DXGIInfoManager::s_instance;

} // namespace jng

#endif
