#pragma once
#include"MyEngine.h"
#include"Camera.h"
#include"MyImGui.h"
class GameScene
{
public:
	void Initialize(DirectXCommon* directX, MyEngine* myEngine, WindowsApp* winApp, int32_t kClientWidth, int32_t kClientHeight);

	void UpDate();

	void Draw();

	void Release();

private:
	int32_t kClientWidth_ = 0;
	int32_t kClientHeight_ = 0;
	DirectXCommon* directX_ = nullptr;
	MyEngine* myEngine_ = nullptr;
	Camera* camera_ = nullptr;
	MyImGui* imGui_ = nullptr;
	Vector4 Left[2] = { -0.5f,-0.5f,0.0f,1.0f };
	Vector4 Top[2] = { 0.0f,0.5f,0.0f,1.0f };
	Vector4 Right[2] = { 0.5f,-0.5f,0.0f,1.0f };
	Vector4 Color[2] = { 1.0f,1.0f,1.0f,1.0f };
	//Sprite
	Vector4 LeftTop = { 0.0f,0.0f,0.0f,1.0f };
	Vector4 LeftBottom = { 0.0f,360.0f,0.0f,1.0f };
	Vector4 RightTop = { 640.0f,0.0f,0.0f,1.0f };
	Vector4 RightBottom = { 640.0f,360.0f,0.0f,1.0f };

};

