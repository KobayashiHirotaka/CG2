#include "WindowsApp.h"
#include "DirectXCommon.h"
#include "MyEngine.h"
#include "GameScene.h"

struct D3DResourceLeakCheker
{
	~D3DResourceLeakCheker()
	{
		Microsoft::WRL::ComPtr<IDXGIDebug1>debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
		{
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
			debug->Release();
		}
	}
};

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	WindowsApp* win = new WindowsApp();
	int32_t kClientWidth = 1280;
	int32_t kClientHeight = 720;

	win->Initialize(kClientWidth, kClientHeight);

	DirectXCommon* dxCommon = new DirectXCommon();
	dxCommon->Initialize(win, kClientWidth, kClientHeight);

	MyEngine* engine = new MyEngine();
	engine->Initialize(dxCommon, kClientWidth, kClientHeight);

	MSG msg{};

	GameScene* gameScene = new GameScene();
	gameScene->Initialize(win, dxCommon, engine, kClientWidth, kClientHeight);

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
			gameScene->UpDate();
			gameScene->Draw();
		}
	}

	gameScene->Release();
	CoUninitialize();


	return 0;
}