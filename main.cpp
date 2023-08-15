#include "WindowsApp.h"
#include "DirectXCommon.h"
#include "MyEngine.h"
#include "Triangle.h"

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	//初期化
	WindowsApp* win = new WindowsApp();
	int32_t kClientWidth = 1280;
	int32_t kClientHeight = 720;

	win->Initialize(kClientWidth, kClientHeight);

	DirectXCommon* dxCommon = new DirectXCommon();
	dxCommon->Initialize(win, kClientWidth, kClientHeight);

	MyEngine* engine = new MyEngine();
	engine->Initialize(dxCommon);

	MSG msg{};

	Triangle* triangle = new Triangle();
	triangle->Initialize(win, dxCommon, engine, kClientWidth, kClientHeight);

	//ウィンドウのxが押されるまでループ
	while (msg.message != WM_QUIT)
	{
		//windowのメッセージを最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲームの処理
			triangle->Draw();
		}
	}

	triangle->Release();
	CoUninitialize();

	return 0;
}