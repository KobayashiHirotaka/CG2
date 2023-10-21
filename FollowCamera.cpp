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
		Vector3 offset = { 0.0f, 2.0f, -10.0f };

		Matrix4x4 cameraRotateMatrix = MakeRotateMatrix(viewProjection_.rotation);

		offset = TransformNormal(offset, cameraRotateMatrix);

		viewProjection_.translation = Add(target_->translation, offset);
	}

	if (Input::GetInstance()->GetJoystickState(joyState_))
	{
		const float kRotateSpeed = 0.02f;

		viewProjection_.rotation.y += (float)joyState_.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;
	}

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();
}
