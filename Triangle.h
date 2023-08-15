#pragma once
#include "MyEngine.h"
#include "MyImGui.h"

class Triangle 
{
public:
	void Initialize(WindowsApp* win, DirectXCommon* dxCommon, MyEngine* engine, int32_t kClientWidth, int32_t kClientHeight);

	void Update();

	void Draw();

	void Release();

private:
	DirectXCommon* dxCommon_;

	MyEngine* engine_;

	MyImGui* imGui_;

	int32_t kClientWidth_ = 0;

	int32_t kClientHeight_ = 0;

	Vector4 triangleData[3][3] =
	{
		{-0.8f,-0.2f,0.0f,1.0f},
	    {-0.6f,0.2f,0.0f,1.0f},
	    {-0.4f,-0.2f,0.0f,1.0f}
	};
	
	Vector4 material[3] = 
	{ 
		{ 1,1,1,1 },
		{ 1,1,1,1 },
		{ 1,1,1,1 } 
	};

	Matrix4x4 wvpData_;

};