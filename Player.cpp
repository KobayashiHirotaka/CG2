#include "Player.h"
#include "GlobalVariables.h"
#include <cassert>

void Player::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	input_ = Input::GetInstance();

	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionMaskPlayer);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	float s = 10;
	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Speed", playerSpeed_);
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
			move = Multiply(playerSpeed_, Normalize(move));

			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation);

			move = TransformNormal(move, rotateMatrix);

			worldTransform_.translation = Add(worldTransform_.translation, move);
			angle_ = std::atan2(move.x, move.z);
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

	worldTransform_.rotation.y = LerpShortAngle(worldTransform_.rotation.y, angle_, 0.01f);

	ICharacter::Update();

	preIsHit_ = isHit_;
	isHit_ = false;
	reStart_ = false;

	Player::ApplyGlobalVariables();
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

void Player::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	playerSpeed_ = globalVariables->GetFloatValue(groupName, "Speed");
}
