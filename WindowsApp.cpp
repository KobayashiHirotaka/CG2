#include "WindowsApp.h"
#include <windows.h>
#include <tchar.h>
#include<d3d12.h>

//コンストラクタ
WindowsApp::WindowsApp()
{
	Title_ = L"CG2_LE2B_13_コバヤシヒロタカ";

	hInst_ = nullptr;
	hwnd_ = nullptr;

	Width_ = 1280;
	Height_ = 720;

	wrc_ = {};
	wc_ = {};
}

//デストラクタ
WindowsApp::~WindowsApp()
{
	End();
}


//開始
void WindowsApp::StartApp()
{
	Initialize();
}

//終了
void WindowsApp::EndApp()
{
	End();
}

// ウィンドウプロシージャ
LRESULT CALLBACK WindowsApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//ウィンドウの初期化
bool WindowsApp::Initialize()
{
	if (!WindowClassRegister())
	{
		return false;
	}
	return true;
}


// ウィンドウクラスの登録
bool WindowsApp::WindowClassRegister()
{
	// ウィンドウプロシージャ
	wc_.lpfnWndProc = WindowProc;
	// ウィンドウクラス名
	wc_.lpszClassName = L"CG2WindowClass";
	// インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	// カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録する
	RegisterClass(&wc_);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	wrc_ = { 0,0,Width_,Height_ };

	//クライアント領域をもとに実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	hwnd_ = CreateWindow
	(
		wc_.lpszClassName,		//利用するクラス名
		Title_,					//タイトルバーの文字(何でも良い)
		WS_OVERLAPPEDWINDOW,	//よく見るウィンドウスタイル
		CW_USEDEFAULT,			//表示X座標(Windowsに任せる)
		CW_USEDEFAULT,			//表示Y座標(WindowsOSに任せる)
		wrc_.right - wrc_.left,	//ウィンドウ横幅
		wrc_.bottom - wrc_.top,	//ウィンドウ縦幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		wc_.hInstance,			//インスタンスハンドル
		nullptr					//オプション
	);

#ifdef _DEBUG

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_))))
	{
		//デバッグレイヤーを有効化する
		debugController_->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController_->SetEnableGPUBasedValidation(TRUE);
	}
#endif

	if (hwnd_ == nullptr)
	{
		return false;
	}

	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);

	return true;
}

void WindowsApp::End()
{
	//ウィンドウの終了処理
	EndWindow();
}

void WindowsApp::EndWindow()
{
#ifdef _DEBUG
	if (debugController_ != nullptr)
	{
		debugController_->Release();
	}
#endif
}