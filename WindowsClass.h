#pragma once
#include <Windows.h>
#include <cstdint>

class WindowsClass
{
public:
	//ウィンドウサイズ
	static const int32_t kClientWidth = 1280;//横幅
	static const int32_t kClientHeight = 720;//縦幅

	//ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	HWND GetHwnd() const { return hwnd_; }

	static void CreateWindowView(const wchar_t* title = L"CG2");

private:
	// ウィンドウクラス
	static inline WNDCLASS wc{};

	//ウィンドウオブジェクト
	static HWND hwnd_;
};
