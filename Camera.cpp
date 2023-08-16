#include "Camera.h"

void Camera::Initialize(int32_t kClientWidth, int32_t kClientHeight)
{
	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth_) / float(kClientHeight_), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	transformationMatrixData = worldViewProjectionMatrix;
}

void Camera::Update()
{
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth_) / float(kClientHeight_), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	transformationMatrixData = worldViewProjectionMatrix;
}

void Camera::ImGui()
{
#pragma region ImGui
	ImGui::Begin("Camera");
	float cameraScale[3] = { cameraTransform.scale.x,cameraTransform.scale.y ,cameraTransform.scale.z };
	ImGui::SliderFloat3("Scale", cameraScale, 1, 30, "%.3f");
	cameraTransform.scale = { cameraScale[0],cameraScale[1],cameraScale[2] };

	float cameraRotate[3] = { cameraTransform.rotate.x,cameraTransform.rotate.y ,cameraTransform.rotate.z };
	ImGui::SliderFloat3("Rotate", cameraRotate, 0, 5, "%.3f");
	cameraTransform.rotate = { cameraRotate[0],cameraRotate[1],cameraRotate[2] };

	float cameraTranslate[3] = { cameraTransform.translate.x,cameraTransform.translate.y ,cameraTransform.translate.z };
	ImGui::SliderFloat3("Translate", cameraTranslate, -10, 10, "%.3f");
	cameraTransform.translate = { cameraTranslate[0],cameraTranslate[1],cameraTranslate[2] };
	ImGui::End();
#pragma endregion
}

