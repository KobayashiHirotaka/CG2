#include "Player.h"
#include <cassert>

void Player::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionMaskPlayer);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	input_ = Input::GetInstance();
}

void Player::Update()
{
	if (preIsHit_ == false && isHit_ == true) 
	{
		worldTransform_.SetParent(parent_);
	}

	if (preIsHit_ == true && isHit_ == false) 
	{
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

	if (isHit_ == false)
	{
		worldTransform_.translation.y -= 0.1f;

	} 
	
	if (isHit_ == true)
	{
		worldTransform_.translation.y = 1.0f;
	}

	if (worldTransform_.translation.y <= -4.0f)
	{
		worldTransform_.translation = { 0.0f,0.0f,0.0f };
	}

	ICharacter::Update();

	preIsHit_ = isHit_;
	isHit_ = false;
	reStart_ = false;
}

void Player::Draw(const ViewProjection& viewProjection)
{
	ICharacter::Draw(viewProjection);
}

void Player::Restart()
{
	reStart_ = true;
	worldTransform_.translation = { 0.0f,0.0f,0.0f };
	worldTransform_.parent_ = nullptr;
}

void Player::OnCollision(Collider* collider)
{
	if (collider->GetCollisionAttribute() & kCollisionAttributeGround && reStart_== false)
	{
		isHit_ = true;
		parent_ = &collider->GetWorldTransform();

		if (worldTransform_.parent_ != parent_)
		{
			worldTransform_.DeleteParent();
			worldTransform_.SetParent(parent_);
		}
	}

	if (collider->GetCollisionAttribute() & kCollisionAttributeEnemy && reStart_ == false)
	{
		Restart();
	}

	if (collider->GetCollisionAttribute() & kCollisionAttributeGoal && reStart_ == false)
	{
		Restart();
	}
}

Vector3 Player::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
	return pos;
}

