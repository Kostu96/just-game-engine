/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#if defined(JNG_DEBUG)

#include "platform/windows/windows_base.hpp"

struct ID3D11Device;
struct IDXGIInfoQueue;

namespace jng {

	void wnd_check_error(HRESULT hr, const char* file, int line);

	namespace wrl = Microsoft::WRL;

    void gfx_check_error(HRESULT hr, const char* file, int line);
    void gfx_check_error_device_removed(HRESULT hr, const wrl::ComPtr<ID3D11Device>& device, const char* file, int line);

	class DXGIInfoManager
	{
	public:
		void checkMessages(const char* file, int line);

		static DXGIInfoManager& get() { return s_instance; }
	private:
		DXGIInfoManager();
		DXGIInfoManager(const DXGIInfoManager&) = delete;
		DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;

		unsigned long long m_next;
		wrl::ComPtr<IDXGIInfoQueue> m_DXGIInfoQueue;
		static DXGIInfoManager s_instance;
	};

} // namespace jng

#define JNG_WND_CHECK_HR(hr) if (FAILED(hr)) { wnd_check_error(hr, __FILE__, __LINE__); }
#define JNG_WND_LAST_ERROR() wnd_check_error(GetLastError(), __FILE__, __LINE__)
#define JNG_D3D_CHECK_HR(hr) if (FAILED(hr)) { gfx_check_error(hr, __FILE__, __LINE__); DXGIInfoManager::get().checkMessages(__FILE__, __LINE__); }
#define JNG_D3D_CHECK() DXGIInfoManager::get().checkMessages(__FILE__, __LINE__)
#define JNG_D3D_CHECK_HR_DEVICE_REMOVED(hr, device) if (FAILED(hr)) { gfx_check_error_device_removed(hr, device, __FILE__, __LINE__); DXGIInfoManager::get().checkMessages(__FILE__, __LINE__); }

#else

#define JNG_WND_CHECK_HR(hr)
#define JNG_WND_LAST_ERROR(hr)
#define JNG_D3D_CHECK_HR(hr)
#define JNG_D3D_CHECK()
#define JNG_D3D_CHECK_HR_DEVICE_REMOVED(hr, device)

#endif
