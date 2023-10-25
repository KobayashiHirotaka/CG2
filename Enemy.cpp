#include "Enemy.h"

void Enemy::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	worldTransform_.Initialize();
	worldTransform_.translation = { 0.0f,1.1f,65.0f };

	move_ = { 0.05f,0.0f,0.0f };

	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionMaskEnemy);
	SetCollisionPrimitive(kCollisionPrimitiveSphere);
}

void Enemy::Update()
{
	ICharacter::Update();

	worldTransform_.translation = Add(worldTransform_.translation, move_);

	if (worldTransform_.translation.x >= 5.0f)
	{
		move_ = { -0.05f,0.0f,0.0f };
	}

	if (worldTransform_.translation.x <= -5.0f)
	{
		move_ = { 0.05f,0.0f,0.0f };
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	ICharacter::Draw(viewProjection);
}

void Enemy::OnCollision(Collider* collider)
{
	isHit_ = true;
	ImGui::Begin("Enemy");
	ImGui::End();
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
	return pos;
}

