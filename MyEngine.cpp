#include "MyEngine.h"

MyEngine* MyEngine::GetInstance()
{
	static MyEngine instance;
	return &instance;
}

void MyEngine::Initialize(DirectXCommon* dxCommon, int32_t kClientWidth, int32_t kClientHeight)
{
	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;
	dxCommon_ = dxCommon;
}

void MyEngine::ImGui()
{
	ImGui::ShowDemoWindow();
}

