#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation = { 0.0f,1.0f,-30.0f };

	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionMaskPlayer);
	SetCollisionPrimitive(kCollisionPrimitiveSphere);

	input_ = Input::GetInstance();
}

void Player::Update()
{
	if (preIsHit_ == false && isHit_ == true) {
		worldTransform_.SetParent(parent_);
	}

	if (preIsHit_ == true && isHit_ == false) {
		worldTransform_.DeleteParent();
	}

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
			const float kPlayerSpeed = 0.3f;

			move = Multiply(kPlayerSpeed, Normalize(move));

			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation);

			move = TransformNormal(move, rotateMatrix);

			worldTransform_.translation = Add(worldTransform_.translation, move);
			worldTransform_.rotation.y = std::atan2(move.x, move.z);
		}
	}

	if (isHit_ == false) {
		worldTransform_.translation.y -= 0.1f;
	}
	else {
		worldTransform_.translation.y = 1.0f;
	}

	if (worldTransform_.translation.y <= -4.0f) {
		worldTransform_.translation = { 0.0f,5.0f,-30.0f };
	}

	worldTransform_.UpdateMatrix();

	preIsHit_ = isHit_;
	isHit_ = false;

	ImGui::Begin("Player");
	ImGui::SliderFloat3("translation", &worldTransform_.translation.x, -3.0f, 10.0f, "%.3f");
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Player::OnCollision(Collider* collider)
{
	isHit_ = true;

	parent_ = &collider->GetWorldTransform();
}

Vector3 Player::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
	return pos;
}

