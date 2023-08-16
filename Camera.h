#pragma once
#include "MyImGui.h"
#include "MyMath.h"
#include "Vector3.h"

class Camera
{
public:
	void Initialize(int32_t kClientWidth, int32_t kClientHeight);

	void Update();

	void ImGui();

	Matrix4x4 transformationMatrixData;

private:
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	Transform cameraTransform
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,-10.0f},
	};

	Transform transform
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
};

