#include "Player.h"
#include "GlobalVariables.h"
#include <cassert>

void Player::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	input_ = Input::GetInstance();

	SetParent(&GetWorldTransform());
	worldTransform_.parent_ = worldTransformHammer_.parent_;

	worldTransformHammer_.Initialize();

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

	if (!Input::GetInstance()->GetJoystickState(joyState_))
	{
		return;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		behaviorRequest_ = Behavior::kAttack;
	}

	if (behaviorRequest_)
	{
		behavior_ = behaviorRequest_.value();

		switch (behavior_)
		{
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;

		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;

		case Behavior::kDash:
			BehaviorDashInitialize();
			break;

		case Behavior::kJump:
			BehaviorJumpInitialize();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_)
	{
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;

	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;

	case Behavior::kDash:
		BehaviorDashUpdate();
		break;

	case Behavior::kJump:
		BehaviorJumpUpdate();
		break;
	}

	if (isHit_ == false)
	{
		worldTransform_.translation.y -= 0.1f;
	} 

	if (worldTransform_.translation.y <= -4.0f)
	{
		worldTransform_.translation = { 0.0f,0.0f,0.0f };
	}

	ICharacter::Update();

	worldTransformHammer_.UpdateMatrix();

	preIsHit_ = isHit_;
	isHit_ = false;
	reStart_ = false;

	Player::ApplyGlobalVariables();
}

void Player::Draw(const ViewProjection& viewProjection)
{
	models_[0]->Draw(worldTransform_, viewProjection);

	if (behavior_ == Behavior::kAttack)
	{
		models_[1]->Draw(worldTransformHammer_, viewProjection);
	}
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
		worldTransform_.translation.y = 1.0f;
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

void Player::BehaviorRootInitialize()
{
	worldTransform_.Initialize();
	worldTransform_.translation.y = 1.0f;
}

void Player::BehaviorRootUpdate()
{
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		isHit_ = false;
		behaviorRequest_ = Behavior::kJump;
	}

	if (Input::GetInstance()->GetJoystickState(joyState_))
	{
		const float deadZone = 0.7f;

		bool isMoving = false;

		velocity_ = { (float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState_.Gamepad.sThumbLY / SHRT_MAX };

		if (Length(velocity_) > deadZone)
		{
			isMoving = true;
		}

		if (isMoving)
		{
			velocity_ = Multiply(playerSpeed_, Normalize(velocity_));

			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation);

			velocity_ = TransformNormal(velocity_, rotateMatrix);

			worldTransform_.translation = Add(worldTransform_.translation, velocity_);
			targetAngle_ = std::atan2(velocity_.x, velocity_.z);
		}
	}

	worldTransform_.rotation.y = LerpShortAngle(worldTransform_.rotation.y, targetAngle_, 0.1f);
}

void Player::BehaviorAttackInitialize()
{
	worldTransformHammer_.rotation.x = 0.0f;

	attackAnimationFrame = 0;
}

void Player::BehaviorAttackUpdate()
{
	if (attackAnimationFrame < 10)
	{
		worldTransformHammer_.rotation.x -= 0.05f;

	}
	else if (worldTransformHammer_.rotation.x <= 2.0f * std::numbers::pi / 4) {
		
		worldTransformHammer_.rotation.x += 0.1f;

	}
	else {
		behaviorRequest_ = Behavior::kRoot;
	}
	attackAnimationFrame++;
}

void Player::BehaviorDashInitialize()
{
	workDash_.dashParameter_ = 0;
	worldTransform_.rotation.y = targetAngle_;
}

void Player::BehaviorDashUpdate()
{
	const uint32_t behaviorDashTime = 20;

	if (++workDash_.dashParameter_ >= behaviorDashTime)
	{
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorJumpInitialize()
{
	worldTransform_.translation.y = 1.0f;

	const float kJumpFirstSpeed_ = 1.0f;

	velocity_.y = kJumpFirstSpeed_;
}

void Player::BehaviorJumpUpdate()
{
	worldTransform_.translation = Add(worldTransform_.translation, velocity_);

	const float kGravityAcceleration_ = 0.05f;

	Vector3 accelerationVector_ = { 0.0f,-kGravityAcceleration_,0.0f };

	velocity_ = Add(velocity_, accelerationVector_);

	if (worldTransform_.translation.y <= 1.0f)
	{
		worldTransform_.translation.y = 1.0f;
		
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	playerSpeed_ = globalVariables->GetFloatValue(groupName, "Speed");
}
