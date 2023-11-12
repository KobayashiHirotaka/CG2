#include "Player.h"
#include "GlobalVariables.h"
#include <cassert>

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

	Matrix4x4 worldMatrix{};
	worldMatrix.m[0][0] = worldTransform_.scale.x * matRot_.m[0][0];
	worldMatrix.m[0][1] = worldTransform_.scale.x * matRot_.m[0][1];
	worldMatrix.m[0][2] = worldTransform_.scale.x * matRot_.m[0][2];
	worldMatrix.m[0][3] = 0.0f;
	worldMatrix.m[1][0] = worldTransform_.scale.y * matRot_.m[1][0];
	worldMatrix.m[1][1] = worldTransform_.scale.y * matRot_.m[1][1];
	worldMatrix.m[1][2] = worldTransform_.scale.y * matRot_.m[1][2];
	worldMatrix.m[1][3] = 0.0f;
	worldMatrix.m[2][0] = worldTransform_.scale.z * matRot_.m[2][0];
	worldMatrix.m[2][1] = worldTransform_.scale.z * matRot_.m[2][1];
	worldMatrix.m[2][2] = worldTransform_.scale.z * matRot_.m[2][2];
	worldMatrix.m[2][3] = 0.0f;
	worldMatrix.m[3][0] = worldTransform_.translation.x;
	worldMatrix.m[3][1] = worldTransform_.translation.y;
	worldMatrix.m[3][2] = worldTransform_.translation.z;
	worldMatrix.m[3][3] = 1.0f;
	worldTransform_.matWorld = worldMatrix;
	//親がいれば行列を掛ける
	if (worldTransform_.parent_) {
		worldTransform_.matWorld = Multiply(worldTransform_.matWorld, worldTransform_.parent_->matWorld);
	}
	worldTransform_.TransferMatrix();

	ICharacter::Update();

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

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
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
			const float playerSpeed = 0.3f;

			move = Multiply(playerSpeed, Normalize(move));

			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation.y);
			move = TransformNormal(move, rotateMatrix);

			targetAngle_ = std::atan2(move.x, move.z);
			matRot_ = DirectionToDirection(Normalize(worldTransform_.translation), Normalize(move));

			worldTransform_.translation = Add(worldTransform_.translation, move);
		}
	}

	worldTransform_.rotation.y = LerpShortAngle(worldTransform_.rotation.y, targetAngle_, 1.0f);
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
	worldTransform_.rotation.y = targetAngle_;
}

void Player::BehaviorDashUpdate()
{
	if (input_->GetJoystickState(joyState_))
	{
		float kSpeed = 1.0f;
		//移動量
		Vector3 move = {
			(float)joyState_.Gamepad.sThumbLX / SHRT_MAX,
			0.0f,
			(float)joyState_.Gamepad.sThumbLY / SHRT_MAX,
		};

		//移動量に速さを反映
		move = Multiply(kSpeed, Normalize(move));

		//移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation.y);
		move = TransformNormal(move, rotateMatrix);

		//移動
		worldTransform_.translation = Add(worldTransform_.translation, move);
	}
	
	if (++workDash_.dashParameter_ >= behaviorDashTime_)
	{
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	behaviorDashTime_ = globalVariables->GetIntValue(groupName, "behaviorDashTime");
}
