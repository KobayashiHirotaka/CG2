#pragma once
#include"MyEngine.h"
#include"Camera.h"
#include"MyImGui.h"

class GameScene
{
public:
	void Initialize(WindowsApp* win, DirectXCommon* dxCommon, MyEngine* engine, int32_t kClientWidth, int32_t kClientHeight);

	void UpDate();

	void Draw();

	void Release();

private:
	int32_t kClientWidth_ = 0;
	int32_t kClientHeight_ = 0;

	DirectXCommon* dxCommon_ = nullptr;
	MyEngine* engine_ = nullptr;
	Camera* camera_ = nullptr;
	MyImGui* imGui_ = nullptr;

	Vector4 triangleData[3] = {};
	Vector4 material[2] = { 1.0f,1.0f,1.0f,1.0f };

	Vector4 LeftTop = { 0.0f,0.0f,0.0f,1.0f };
	Vector4 LeftBottom = { 0.0f,360.0f,0.0f,1.0f };
	Vector4 RightTop = { 640.0f,0.0f,0.0f,1.0f };
	Vector4 RightBottom = { 640.0f,360.0f,0.0f,1.0f };
};

