#include"GameScene.h"

void GameScene::Initialize(WindowsApp* win, DirectXCommon* dxCommon, MyEngine* myEngine, int32_t kClientWidth, int32_t kClientHeight)
{
	dxCommon_ = dxCommon;
	myEngine_ = myEngine;

	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;

	camera_ = new Camera();
	camera_->Initialize(kClientWidth_, kClientHeight_);

	imGui_ = new MyImGui();
	imGui_->Initialize(win, dxCommon_);

	myEngine_->LoadTexture("resource/uvChecker.png");

	triangleData[0] = { -0.5f,-0.5f,0.0f,1.0f };
	triangleData[1] = { 0.0f,0.5f,0.0f,1.0f };
	triangleData[2] = { 0.5f,-0.5f,0.0f,1.0f };
}

void GameScene::UpDate()
{
	camera_->Update();
}

void GameScene::Draw()
{

	imGui_->BeginFlame();
	dxCommon_->PreDraw();

	ImGui::Begin("TriAngleColor");

	float color[3] = { material[0].x,material[0].y ,material[0].w };
	ImGui::SliderFloat3("RGB", color, 0, 1, "%.3f");
	ImGui::ColorEdit3("MaterialColor", color);
	material[0] = { color[0],color[1],color[2] };

	ImGui::End();

	myEngine_->ImGui();

	myEngine_->Draw(triangleData[0], triangleData[1], triangleData[2], material[0], camera_->transformationMatrixData);
	myEngine_->DrawSprite(LeftTop, LeftBottom, RightTop, RightBottom);

	imGui_->EndFlame();
	dxCommon_->PostDraw();
}

void GameScene::Release()
{
	ImGui_ImplDX12_Shutdown();
	myEngine_->Release();
	dxCommon_->Release();
}
