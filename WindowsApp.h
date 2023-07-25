#pragma once
#include <windows.h>
#include<cstdint>
#include<d3d12.h>
#include<dxgidebug.h>

#pragma comment(lib,"dxguid.lib")

class WindowsApp
{
public:
	//コンストラクタ
	WindowsApp();
	//デストラクタ
	~WindowsApp();
	//アプリケーションの開始
	void StartApp();
	//アプリケーションの終了
	void EndApp();

	int32_t GetWidth() const { return Width_; };
	int32_t GetHeight() const { return Height_; };
	HWND GetHwnd() const { return hwnd_; }

private:
	const wchar_t* Title_; 
	HINSTANCE hInst_;	
	HWND hwnd_;			
	int32_t Width_;		// ウィンドウの横幅
	int32_t Height_;	// ウィンドウの縦幅

	RECT wrc_;

	// ウィンドウクラスの登録
	WNDCLASS wc_;

	ID3D12Debug1* debugController_ = nullptr;

	// 初期化
	bool Initialize();

	// アプリケーションの終了
	void End();

	// ウィンドウクラスを登録
	bool WindowClassRegister();

	// ウィンドウの終了
	void EndWindow();

	// ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};