#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();


	worldTransform_.translation = { -12.0f,1.0f,-50.0f };

	input_ = Input::GetInstance();
}

void Player::Update()
{
	worldTransform_.TransferMatrix();

	if (Input::GetInstance()->GetJoystickState(joyState_))
	{
		const float deadZone = 0.7f;

		bool isMoving = false;

		Vector3 move = { (float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState_.Gamepad.sThumbLY / SHRT_MAX };

		if (Length(move) > deadZone)
		{
			isMoving = true;
		}

		if (isMoving)
		{
			const float kPlayerSpeed = 0.2f;

			move = Multiply(kPlayerSpeed, Normalize(move));

			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation);

			move = TransformNormal(move, rotateMatrix);

			worldTransform_.translation = Add(worldTransform_.translation, move);
			worldTransform_.rotation.y = std::atan2(move.x, move.z);
		}
	}

	worldTransform_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::SliderFloat3("translation", &worldTransform_.translation.x, -3.0f, 10.0f, "%.3f");
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}


