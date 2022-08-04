/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/platform.hpp"

#include "core/engine.hpp"
#include "platform/window.hpp"
#include "platform/windows/windows_base.hpp"

#include <shlobj.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace jng {
    namespace Platform
    {
        std::string openDirectoryDialog()
        {
            TCHAR previousDir[MAX_PATH]{};
            GetCurrentDirectory(MAX_PATH, previousDir);

            TCHAR buffer[MAX_PATH]{};
            BROWSEINFO info{};
            info.hwndOwner = glfwGetWin32Window(Engine::get().getWindow().getNativeWindowHandle());;
            info.lpszTitle = TEXT("Browse Directory");
            info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
            LPITEMIDLIST itemIDList = SHBrowseForFolder(&info);

            if (itemIDList)
            {
                // get the name of the folder and put it in path
                SHGetPathFromIDList(itemIDList, buffer);

                //Set the current directory to path
                SetCurrentDirectory(buffer);

                //Begin the search
                //SearchFolder(buffer);

                // free memory used
                IMalloc* imalloc = 0;
                if (SUCCEEDED(SHGetMalloc(&imalloc)))
                {
                    imalloc->Free(itemIDList);
                    imalloc->Release();
                }

                SetCurrentDirectory(previousDir);
                return buffer;
            }

            return "";
        }

        std::string openFilenameDialog(const char* filter)
        {
            TCHAR buffer[MAX_PATH]{};

            OPENFILENAME openFilename{};
            openFilename.lStructSize = sizeof(OPENFILENAME);
            openFilename.hwndOwner = glfwGetWin32Window(Engine::get().getWindow().getNativeWindowHandle());
            openFilename.lpstrFilter = filter;
            openFilename.nFilterIndex = 1;
            openFilename.lpstrFile = buffer;
            openFilename.nMaxFile = sizeof(buffer);
            openFilename.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

            if (GetOpenFileName(&openFilename) == TRUE)
                return openFilename.lpstrFile;

            return "";
        }

        std::string saveFilenameDialog(const char* filter)
        {
            TCHAR buffer[MAX_PATH]{};

            OPENFILENAME openFilename{};
            openFilename.lStructSize = sizeof(OPENFILENAME);
            openFilename.hwndOwner = glfwGetWin32Window(Engine::get().getWindow().getNativeWindowHandle());
            openFilename.lpstrFilter = filter;
            openFilename.nFilterIndex = 1;
            openFilename.lpstrFile = buffer;
            openFilename.nMaxFile = sizeof(buffer);
            openFilename.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

            if (GetSaveFileName(&openFilename) == TRUE)
                return openFilename.lpstrFile;

            return "";
        }

    } // namespace Platform
} // namespace jng
