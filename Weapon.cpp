#include "Weapon.h"
#include <cassert>

void Weapon::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	
	worldTransform_.Initialize();
	worldTransform_.translation.y = 0.8f;
	
	SetCollisionAttribute(kCollisionAttributeWeapon);
	SetCollisionMask(kCollisionMaskWeapon);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabbSize = { {-10.0f,-10.0f,-10.0f},{10.0f,10.0f,10.0f} };
	SetAABB(aabbSize);
}

void Weapon::Update()
{
	if (isAttack_)
	{
		attackAnimationTimer_++;
	}

	if (attackAnimationCount_ == 0) 
	{
		if (attackAnimationTimer_ == 30) 
		{
			attackAnimationCount_++;
			attackAnimationTimer_ = 0;
		}

		worldTransform_.rotation.x -= 0.1f;
	}

	if (attackAnimationCount_ == 1)
	{
		if (attackAnimationTimer_ == 15)
		{
			attackAnimationCount_++;
			attackAnimationTimer_ = 0;
		}

		worldTransform_.rotation.x += 0.2f;
	}

	if (attackAnimationCount_ == 2)
	{
		if (attackAnimationTimer_ == 30) 
		{
			attackAnimationCount_++;
			attackAnimationTimer_ = 0;
			isAttack_ = false;
		}
	}

	worldTransform_.UpdateMatrix();
}

void Weapon::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Weapon::OnCollision(Collider* collider) 
{
	ImGui::Begin("Weapon");
	ImGui::Text("Hit");
	ImGui::End();
}

Vector3 Weapon::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
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