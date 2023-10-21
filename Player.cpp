#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	input_ = Input::GetInstance();
	worldTransform_.Initialize();
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

			worldTransform_.translation = Add(worldTransform_.translation, move);
		}
	}

	worldTransform_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::Text("DebugCamera ENTER");
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}


