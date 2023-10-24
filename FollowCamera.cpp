#include "FollowCamera.h"

void FollowCamera::Initialize()
{
	viewProjection_.Initialize();

	input_ = Input::GetInstance();
}

void FollowCamera::Update()
{
	//追従対象がいれば
	if (target_)
	{
		//追従対象からカメラまでのオフセット
		Vector3 offset = { 0.0f, 2.0f, -25.0f };

		Matrix4x4 cameraRotateMatrix = MakeRotateMatrix(viewProjection_.rotation);

		offset = TransformNormal(offset, cameraRotateMatrix);

		viewProjection_.translation = Add(GetWorldPosition(), offset);
	}

	if (Input::GetInstance()->GetJoystickState(joyState_))
	{
		const float deadZone = 0.7f;

		bool isMoving = false;

		Vector3 move = { 0.0f, (float)joyState_.Gamepad.sThumbRX / SHRT_MAX, 0.0f };

		if (Length(move) > deadZone)
		{
			isMoving = true;
		}

		if (isMoving)
		{
			const float kRotateSpeed = 0.02f;

			viewProjection_.rotation.y += move.y * kRotateSpeed;
		}
	}

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	ImGui::Begin("camera");
	ImGui::DragFloat3("trans", &viewProjection_.translation.x, 0.1f);
	ImGui::DragFloat3("rotate", &viewProjection_.rotation.x, 0.1f);
	ImGui::End();
}

Vector3 FollowCamera::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = target_->matWorld.m[3][0];
	pos.y = target_->matWorld.m[3][1];
	pos.z = target_->matWorld.m[3][2];
	return pos;
}
