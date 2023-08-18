#include"GameScene.h"

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

	uvChecker = engine_->LoadTexture("resource/uvChecker.png");
	monsterBall = engine_->LoadTexture("resource/monsterBall.png");

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

	ImGui::Begin("sphereTexture");
	ImGui::Checkbox("texture", &changeTexture);
	ImGui::End();

	engine_->ImGui();

	if (changeTexture == true)
	{
		sphereTexture = monsterBall;

	}else {
		sphereTexture = uvChecker;
	}
	
	engine_->Draw(triangleData[0], triangleData[1], triangleData[2], material[0], camera_->transformationMatrixData, uvChecker);
	engine_->DrawSprite(LeftTop, LeftBottom, RightTop, RightBottom, uvChecker);
	engine_->DrawSphere(sphere, camera_->transformationMatrixData,sphereTexture);

	imGui_->EndFlame();
	dxCommon_->PostDraw();
}

void GameScene::Release()
{
	ImGui_ImplDX12_Shutdown();
	engine_->Release();
	dxCommon_->Release();
}
