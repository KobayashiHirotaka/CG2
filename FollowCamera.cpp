#include "FollowCamera.h"
#include "LockOn.h"
#include "GlobalVariables.h"

void FollowCamera::Initialize()
{
	input_ = Input::GetInstance();

	viewProjection_.Initialize();

	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "FollowCamera";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "cameraDelay", cameraDelay_);
}

void FollowCamera::Update()
{
	if (target_) 
	{
		interTarget_ = Lerp(interTarget_, GetWorldPosition(), cameraDelay_);
	}

	Vector3 offset = FollowCamera::Offset();
	
	viewProjection_.translation = Add(interTarget_, offset);

	if (lockOn_ && lockOn_->ExistTarget()) {
		
		Vector3 lockOnPosition = lockOn_->GetTargetPosition();

		Vector3 sub = Subtract(lockOnPosition, GetWorldPosition());

		if (sub.z != 0.0) {
			destinationAngleY_ = std::asin(sub.x / std::sqrt(sub.x * sub.x + sub.z * sub.z));

			if (sub.z < 0.0) {
				destinationAngleY_ = (sub.x >= 0.0) ? 3.141592f - destinationAngleY_ : -3.141592f - destinationAngleY_;
			}
		}
		else {
			destinationAngleY_ = (sub.x >= 0.0) ? 3.141592f / 2.0f : -3.141592f / 2.0f;
		}

	}
	else {
		if (input_->GetJoystickState())
		{
			const float deadZone = 0.7f;

			bool isMoving = false;

			Vector3 move = { 0.0f, Input::GetInstance()->GetRightStickX(), 0.0f };

			if (Length(move) > deadZone)
			{
				isMoving = true;
			}

			if (isMoving)
			{
				const float kRotSpeedY = 0.04f;

				destinationAngleY_ += move.y * kRotSpeedY;
			}
		}
	}

	viewProjection_.rotation.y = LerpShortAngle(viewProjection_.rotation.y, destinationAngleY_, 0.1f);

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	FollowCamera::ApplyGlobalVariables();
}

Vector3 FollowCamera::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = target_->matWorld.m[3][0];
	pos.y = target_->matWorld.m[3][1];
	pos.z = target_->matWorld.m[3][2];
	return pos;
}

void FollowCamera::SetTarget(const WorldTransform* target)
{ 
	target_ = target; 
	FollowCamera::Reset();
}

Vector3 FollowCamera::Offset()
{
	Vector3 offset = { 0.0f, 2.0f, -25.0f };
	
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(viewProjection_.rotation.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_.rotation.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(viewProjection_.rotation.z);
	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	offset = TransformNormal(offset, rotateMatrix);

	return offset;
}

void FollowCamera::Reset() 
{
	if (target_)
	{
		interTarget_ = target_->translation;
		viewProjection_.rotation.x = target_->rotation.x;
		viewProjection_.rotation.y = target_->rotation.y;
	}

	destinationAngleY_ = viewProjection_.rotation.y;

	Vector3 offset = FollowCamera::Offset();
	viewProjection_.translation = Add(interTarget_, offset);
}

void FollowCamera::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "FollowCamera";

	cameraDelay_ = globalVariables->GetFloatValue(groupName, "cameraDelay");
}