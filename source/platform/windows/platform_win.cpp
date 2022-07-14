/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/platform.hpp"

#include "core/engine.hpp"
#include "platform/window.hpp"
#include "platform/windows/windows_base.hpp"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace jng {
	namespace Platform
	{
		// TODO: checkout new windows Common Item Dialog
		std::string openFilenameDialog(const char* filter)
		{
			TCHAR buffer[256]{};

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

			JNG_CORE_ERROR("openFilenameDialogError");
			return "";
		}

		// TODO: checkout new windows Common Item Dialog
		std::string saveFilenameDialog(const char* filter)
		{
			TCHAR buffer[256]{};

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

			JNG_CORE_ERROR("saveFilenameDialogError");
			return "";
		}

	} // namespace Platform
} // namespace jng
