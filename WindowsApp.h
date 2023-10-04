#pragma once
#include <Windows.h>
#include <cstdint>
#include "ConvertString.h"
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class WindowsApp 
{
public:
	static WindowsApp* GetInstance();

	void Initialize();

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	HWND GetHwnd() const{ return hwnd_; }

	HINSTANCE GetHinstance()const { return wc_.hInstance; }

	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

private:
	WindowsApp() = default;
	~WindowsApp() = default;
	WindowsApp(const WindowsApp& obj) = delete;
	WindowsApp& operator=(const WindowsApp& obj) = delete;

	WNDCLASS wc_{};
	HWND hwnd_ = nullptr;
};
