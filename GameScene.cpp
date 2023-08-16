#include "GameScene.h"

void GameScene::Initialize(WindowsApp* win, DirectXCommon* dxCommon, MyEngine* engine, int32_t kClientWidth, int32_t kClientHeight)
{
	dxCommon_ = dxCommon;
	engine_ = engine;

	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;

	camera_ = new Camera();
	camera_->Initialize(kClientWidth_, kClientHeight_);

	imGui_ = new MyImGui();
	imGui_->Initialize(win, dxCommon_);

	engine_->LoadTexture("resource/uvChecker.png");
}

void GameScene::Update()
{
	camera_->Update();
}

void GameScene::Draw()
{
	imGui_->BeginFrame();
	dxCommon_->PreDraw();

	engine_->Draw(triangleData[0][0], triangleData[0][1], triangleData[0][2], material[0], camera_->transformationMatrixData);
	engine_->DrawSprite(LeftTop, LeftBottom, RightTop, RightBottom);

	imGui_->EndFlame();
	dxCommon_->PostDraw();
}

void GameScene::Release()
{
	ImGui_ImplDX12_Shutdown();

	dxCommon_->Release();
	engine_->Release();
}