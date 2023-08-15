#include "Triangle.h"

void Triangle::Initialize(WindowsApp* win, DirectXCommon* dxCommon, MyEngine* engine, int32_t kClientWidth, int32_t kClientHeight)
{
	dxCommon_ = dxCommon;
	engine_ = engine;

	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;

	imGui_ = new MyImGui();
	imGui_->Initialize(win, dxCommon_);

	engine_->LoadTexture("resource/uvChecker.png");
}

void Triangle::Update()
{

}

void Triangle::Draw()
{
	imGui_->BeginFrame();
	dxCommon_->PreDraw();

	engine_->Draw(triangleData[0][0], triangleData[0][1], triangleData[0][2], material[0], wvpData_);

	imGui_->EndFlame();
	dxCommon_->PostDraw();
}

void Triangle::Release()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	dxCommon_->Release();
	engine_->Release();
}