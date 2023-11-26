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
		{-collisionWorldTransform_.scale.x,-collisionWorldTransform_.scale.y,-collisionWorldTransform_.scale.z + 2.0f},
		{collisionWorldTransform_.scale.x,collisionWorldTransform_.scale.y,collisionWorldTransform_.scale.z + 2.0f},
	};

	SetAABB(aabbSize);
}

void Weapon::Update()
{
	Vector3 direction{ 0.0f,0.0f,4.0f };
	direction = TransformNormal(direction, worldTransform_.parent_->matWorld);
	collisionWorldTransform_.translation = { worldTransform_.matWorld.m[3][0],worldTransform_.matWorld.m[3][1],worldTransform_.matWorld.m[3][2] };
	collisionWorldTransform_.translation = Add(collisionWorldTransform_.translation, direction);


	worldTransform_.UpdateMatrix(RotationType::Euler);
	collisionWorldTransform_.UpdateMatrix(RotationType::Euler);
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