#include "SceneManager.h"

void SceneManager::Initialize()
{
	//COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	win_ = WindowsApp::GetInstance();

	dxCommon_ = DirectXCommon::GetInstance();

	engine_ = MyEngine::GetInstance();
	engine_->Initialize();

	textureManager_ = TextureManager::GetInstance();

	imGui_ = new MyImGui();
	imGui_->Initialize(win_, dxCommon_);

	input_ = Input::GetInstance();
	input_->Initialize(win_);

	audio_ = Audio::GetInstance();
	audio_->Initialize();

	light_ = Light::GetInstance();
	light_->Initialize();

	state[START] = std::make_unique<GameStartScene>();
	state[PLAY] = std::make_unique<GamePlayScene>();
	state[PLAY]->Initialize();

	IScene::stateNum = START;
}

void SceneManager::UpDate()
{
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
			imGui_->BeginFlame();
			dxCommon_->PreDraw();
			input_->Update();

			state[GameStartScene::stateNum]->Update();
			state[GameStartScene::stateNum]->Draw();

			imGui_->EndFlame();
			dxCommon_->PostDraw();
		}
	}
	Release();
}

void SceneManager::Release()
{
	ImGui_ImplDX12_Shutdown();
	CoUninitialize();
}