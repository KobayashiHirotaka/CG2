#include"GameScene.h"

void GameScene::Initialize()
{
	//COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	win_ = new WindowsApp();
	kClientWidth_ = 1280;
	kClientHeight_ = 720;

	win_->Initialize(kClientWidth_, kClientHeight_);

	dxCommon_ = new DirectXCommon();
	dxCommon_->Initialize(win_, kClientWidth_, kClientHeight_);

	engine_ = MyEngine::GetInstance();
	engine_->Initialize(dxCommon_, kClientWidth_, kClientHeight_);

	imGui_ = new MyImGui();
	imGui_->Initialize(win_, dxCommon_);

	input_ = Input::GetInstance();
	input_->Initialize(win_);

	audio_ = Audio::GetInstance();
	audio_->Initialize();

	state[START] = std::make_unique<GameStartScene>();
	state[PLAY] = std::make_unique<GamePlayScene>();
	state[PLAY]->Initialize();

	IScene::stateNum = START;
}

void GameScene::UpDate()
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
			input_->Update();
			dxCommon_->PreDraw();

			state[GameStartScene::stateNum]->Update();
			state[GameStartScene::stateNum]->Draw();

			imGui_->EndFlame();
			dxCommon_->PostDraw();
		}
	}
	Release();
}

void GameScene::Release()
{
	ImGui_ImplDX12_Shutdown();
	CoUninitialize();
}