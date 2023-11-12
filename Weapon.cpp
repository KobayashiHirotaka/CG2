#include "Weapon.h"
#include <cassert>

void Weapon::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	
	worldTransform_.Initialize();
	collisionWorldTransform_.Initialize();
	
	SetCollisionAttribute(kCollisionAttributeWeapon);
	SetCollisionMask(kCollisionMaskWeapon);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabbSize =
	{
		{-collisionWorldTransform_.scale.x,-collisionWorldTransform_.scale.y,-collisionWorldTransform_.scale.z},
		{collisionWorldTransform_.scale.x,collisionWorldTransform_.scale.y,collisionWorldTransform_.scale.z},
	};

	SetAABB(aabbSize);
}

void Weapon::Update()
{
	if (isAttack_ == true)
	{
		attackAnimationTimer_++;
	}

	if (attackAnimationCount_ == 0)
	{
		if (attackAnimationTimer_ == 30)
		{
			attackAnimationCount_ = 1;
			attackAnimationTimer_ = 0;
		}

		worldTransform_.rotation.x -= 0.1f;
	}

	if (attackAnimationCount_ == 1)
	{
		if (attackAnimationTimer_ == 15)
		{
			attackAnimationCount_ = 2;
			attackAnimationTimer_ = 0;
		}

		worldTransform_.rotation.x += 0.2f;

		Vector3 direction{ 0.0f,0.0f,4.0f };
		direction = TransformNormal(direction, worldTransform_.matWorld);
		collisionWorldTransform_.translation = { worldTransform_.matWorld.m[3][0],worldTransform_.matWorld.m[3][1],worldTransform_.matWorld.m[3][2] };
		collisionWorldTransform_.translation = Add(collisionWorldTransform_.translation, direction);
	}

	if (attackAnimationCount_ == 2)
	{
		if (attackAnimationTimer_ == 10)
		{
			attackAnimationTimer_ = 0;
			isAttack_ = false;
		}
	}

	worldTransform_.UpdateMatrix();
	collisionWorldTransform_.UpdateMatrix();
}

void Weapon::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Weapon::OnCollision(Collider* collider) 
{
	
}

Vector3 Weapon::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = collisionWorldTransform_.matWorld.m[3][0];
	pos.y = collisionWorldTransform_.matWorld.m[3][1];
	pos.z = collisionWorldTransform_.matWorld.m[3][2];
	return pos;
}

void Weapon::AttackInitialize()
{
	attackAnimationTimer_ = 0;
	attackAnimationCount_ = 0;
	worldTransform_.rotation.x = 1.5f;
}

void Weapon::Attack() 
{
	if (isAttack_ == false) 
	{
		Weapon::AttackInitialize();
		isAttack_ = true;
	}
}