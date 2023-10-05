#include "Camera.h"

void Camera::Initialize(int32_t kClientWidth, int32_t kClientHeight)
{
	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;

	matRot_ = MakeIdentity4x4();
	worldMatrix = MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, { 0,0,0 });

	Matrix4x4 cameraMatrix = MakeIdentity4x4();
	cameraMatrix = Multiply(cameraMatrix, matRot_);

	Matrix4x4 Move = MakeTranslateMatrix(translation_);
	cameraMatrix = Multiply(cameraMatrix, Move);
	ViewMatrix = Inverse(cameraMatrix);
	ViewMatrix = Multiply(ViewMatrix, matRot_);
	ProjectionMatrix = MakePerspectiveFovMatrix(FOV, float(kClientWidth_) / float(kClientHeight_), 0.1f, 100.0f);

	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(ViewMatrix, ProjectionMatrix));
	transformationMatrixData = worldViewProjectionMatrix;
	input = Input::GetInstance();
}

void Camera::Update()
{
#ifdef _DEBUG
	if (DebucCameraFlag) 
	{
		DebugCameraMove();
	}
#endif // _DEBUG

	worldMatrix = MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, { 0,0,0 });
	
	Matrix4x4 cameraMatrix = MakeIdentity4x4();
	cameraMatrix = Multiply(cameraMatrix, matRot_);

	Matrix4x4 rotate = MakeRotateMatrix(rotation_);
	cameraMatrix = Multiply(cameraMatrix, rotate);

	Matrix4x4 Move = MakeTranslateMatrix(translation_);
	cameraMatrix = Multiply(cameraMatrix, Move);
	ViewMatrix = Inverse(cameraMatrix);

	ProjectionMatrix = MakePerspectiveFovMatrix(FOV, float(kClientWidth_) / float(kClientHeight_), 0.1f, 100.0f);

	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(ViewMatrix, ProjectionMatrix));
	transformationMatrixData = worldViewProjectionMatrix;

	ImGui::Begin("DebugCamera");
	ImGui::SliderFloat3("Rotate", &rotation_.x, -5, 5, "%.3f");
	ImGui::SliderFloat3("Translate", &translation_.x, -10, 10, "%.3f");
	ImGui::Text("DebugCamera %d", DebucCameraFlag);
	ImGui::Text("DebugCameraOn : 1\n");
	ImGui::Text("DebugCameraOff : 2\n");
	ImGui::End();
}

Vector3 Camera::GetmatRot()
{
	Vector3 result;
	result.x = matRot_.m[0][0] * matRot_.m[0][1] * matRot_.m[0][2];
	result.y = matRot_.m[0][0] * matRot_.m[0][2] * matRot_.m[2][0] * matRot_.m[2][2];
	result.z = matRot_.m[0][0] * matRot_.m[0][1] * matRot_.m[1][0] * matRot_.m[1][1];

	return result;
}

#ifdef _DEBUG
void Camera::DebugCameraMove()
{
	Matrix4x4 matRotDelta = MakeIdentity4x4();

	if (input->PressKey(DIK_UP))
	{
		const float speed = -0.01f;
		matRotDelta = Multiply(matRotDelta, MakeRotateXMatrix(speed));
	}

	if (input->PressKey(DIK_DOWN)) 
	{
		const float speed = 0.01f;
		matRotDelta = Multiply(matRotDelta, MakeRotateXMatrix(speed));
	}

	if (input->PressKey(DIK_LEFT))
	{
		const float speed = -0.01f;
		matRotDelta = Multiply(matRotDelta, MakeRotateYMatrix(speed));
	}

	if (input->PressKey(DIK_RIGHT))
	{
		const float speed = 0.01f;
		matRotDelta = Multiply(matRotDelta, MakeRotateYMatrix(speed));
	}

	matRot_ = Multiply(matRotDelta, matRot_);

	if (input->PressKey(DIK_A))
	{
		Vector3 translate{ speed,0,0 };
		translation_ = Subtract(translation_, translate);
	}

	if (input->PressKey(DIK_D)) 
	{
		Vector3 translate{ speed,0,0 };
		translation_ = Add(translation_, translate);
	}

	if (input->PressKey(DIK_W))
	{
		Vector3 translate{ 0,speed,0 };
		translation_ = Add(translation_, translate);
	}

	if (input->PressKey(DIK_S))
	{
		Vector3 translate{ 0,speed,0 };
		translation_ = Subtract(translation_, translate);
	}

	if (input->PressKey(DIK_E)) 
	{
		Vector3 translate{ 0,0,speed };
		translation_ = Add(translation_, translate);
	}

	if (input->PressKey(DIK_Q))
	{
		Vector3 translate{ 0,0,speed };
		translation_ = Subtract(translation_, translate);
	}
}
#endif // DEBUG