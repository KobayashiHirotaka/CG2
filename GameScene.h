#pragma once
#include "MyEngine.h"
#include "MyImGui.h"
#include "Camera.h"

class GameScene
{
public:
	void Initialize(WindowsApp* win, DirectXCommon* dxCommon, MyEngine* engine, int32_t kClientWidth, int32_t kClientHeight);

	void Update();

	void Draw();

	void Release();

private:
	DirectXCommon* dxCommon_;

	MyEngine* engine_;

	Camera* camera_;

	MyImGui* imGui_;

	int32_t kClientWidth_ = 0;

	int32_t kClientHeight_ = 0;

	Vector4 triangleData[3][3] =
	{
		{-0.5f,-0.5f,0.0f,1.0f },
	    {0.0f,0.5f,0.0f,1.0f},
	    {0.5f,-0.5f,0.0f,1.0f}
	};
	
	Vector4 material[3] = { { 1,1,1,1 },};

	Vector4 LeftTop = { 0.0f,0.0f,0.0f,1.0f };
	Vector4 LeftBottom = { 0.0f,360.0f,0.0f,1.0f };
	Vector4 RightTop = { 640.0f,0.0f,0.0f,1.0f };
	Vector4 RightBottom = { 640.0f,360.0f,0.0f,1.0f };
};