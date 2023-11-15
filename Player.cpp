#include "Player.h"
#include "GlobalVariables.h"
#include <cassert>

const std::array<Player::ConstAttack, Player::ComboNum>
Player::kConstAttacks_ = {
	{
	{0,0,20,0,0.0f,0.0f,0.15f},
	{15,10,15,0,0.2f,0.0f,0.0f},
	{15,10,15,30,0.2f,0.0f,0.0f},
	}
};

void Player::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	input_ = Input::GetInstance();

	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize(models_[1]);
	weapon_->SetParent(&worldTransform_);

	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionMaskPlayer);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "behaviorDashTime", behaviorDashTime_);
}

void Player::Update()
{
	if (!Input::GetInstance()->GetJoystickState(joyState_))
	{
		return;
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

	worldTransform_.quaternion = Slerp(worldTransform_.quaternion,moveQuaternion_,0.2f);
	worldTransform_.quaternion = Normalize(worldTransform_.quaternion);
	worldTransform_.UpdateMatrix(RotationType::Quaternion);

	weapon_->Update();

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
		weapon_->Draw(viewProjection);
	}
}

void Player::Restart()
{
	reStart_ = true;
	worldTransform_.translation = { 0.0f,1.0f,0.0f };
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
	
}

void Player::BehaviorRootUpdate()
{
	if (workDash_.coolTime != 60)
	{
		workDash_.coolTime++;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		if (workDash_.coolTime == 60)
		{
			behaviorRequest_ = Behavior::kDash;
		}
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		if (workDash_.coolTime == 60)
		{
			behaviorRequest_ = Behavior::kAttack;
		}
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
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
			const float playerSpeed = 0.3f;

			velocity_ = Multiply(playerSpeed, Normalize(velocity_));

			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation.y);
			velocity_ = TransformNormal(velocity_, rotateMatrix);

			worldTransform_.translation = Add(worldTransform_.translation, velocity_);

			velocity_ = Normalize(velocity_);
			Vector3 cross = Normalize(Cross({0.0f,0.0f,1.0f}, velocity_));
			float dot = Dot({ 0.0f,0.0f,1.0f }, velocity_);
			moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
		}
	}
}

void Player::BehaviorAttackInitialize()
{
	weapon_->Attack();
}

void Player::BehaviorAttackUpdate()
{
	if (weapon_->GetIsAttack() == false)
	{
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorDashInitialize()
{
	workDash_.dashParameter_ = 0;
	workDash_.coolTime = 0;
}

void Player::BehaviorDashUpdate()
{
	if (input_->GetJoystickState(joyState_))
	{
		float kSpeed = 1.0f;
		//移動量
		Vector3 move_ = {
			(float)joyState_.Gamepad.sThumbLX / SHRT_MAX,
			0.0f,
			(float)joyState_.Gamepad.sThumbLY / SHRT_MAX,
		};

		//移動量に速さを反映
		move_ = Multiply(kSpeed, Normalize(move_));

		//移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation.y);
		move_ = TransformNormal(move_, rotateMatrix);

		//移動
		worldTransform_.translation = Add(worldTransform_.translation, move_);
	}
	
	if (++workDash_.dashParameter_ >= behaviorDashTime_)
	{
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorJumpInitialize()
{
	isHit_ = false;

	worldTransform_.translation.y = 1.0f;

	const float kJumpFirstSpeed_ = 1.0f;

	velocity_ = { (float)joyState_.Gamepad.sThumbLX / (SHRT_MAX * 2), kJumpFirstSpeed_, (float)joyState_.Gamepad.sThumbLY / (SHRT_MAX * 2) };
}

void Player::BehaviorJumpUpdate()
{
	worldTransform_.translation = Add(worldTransform_.translation, velocity_);

	const float kGravityAcceleration_ = 0.05f;

	Vector3 accelerationVector_ = { 0.0f,-kGravityAcceleration_,0.0f };

	velocity_ = Add(velocity_, accelerationVector_);

	if (isHit_ == true)
	{
		worldTransform_.translation.y = 1.0f;

		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	behaviorDashTime_ = globalVariables->GetIntValue(groupName, "behaviorDashTime");
}
