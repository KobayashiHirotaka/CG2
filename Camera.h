#pragma once
#include "MyImGui.h"
#include "MyMath.h"
#include "Vector3.h"
#include "Input.h"

class Camera
{
public:
	void Initialize(int32_t kClientWidth, int32_t kClientHeight);

	void Update();

	const Matrix4x4& GetTransformationMatrix() const { return transformationMatrixData; }

	Vector3 GetmatRot();

	Matrix4x4 transformationMatrixData;

#ifdef _DEBUG
	void DebugCameraMove();

	void DebugCamera(bool Flag) 
	{
		DebucCameraFlag = Flag;
	}
#endif // DEBUG

private:
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0,0,0 };

	// 累積回転行列
	Matrix4x4 matRot_;

	// ローカル座標
	Vector3 translation_ = { 0,0,-5 };

	//　ビュー行列
	Matrix4x4 ViewMatrix;

	// 射影行列
	Matrix4x4 ProjectionMatrix;
	Matrix4x4 worldMatrix;

	Input* input = nullptr;

	//FOV
	float FOV = 45.0f;

#ifdef _DEBUG
	bool DebucCameraFlag = false;
#endif // DEBUG
};

