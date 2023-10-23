#include "MoveGround.h"
#include <cassert>

void MoveGround::Initialize(Model* model, const Vector3& position)
{
	assert(model);
	model_ = model;
	worldTransform_.translation = position;
	worldTransform_.Initialize();

	move_ = { 0.0f,0.0f,0.05f };

	SetCollisionAttribute(CollisionConfig::kCollisionAttributeGround);
	SetCollisionMask(~CollisionConfig::kCollisionAttributeGround);
}

void MoveGround::Update()
{
	worldTransform_.translation = Add(worldTransform_.translation, move_);

	if (worldTransform_.translation.z >= 20.0f)
	{
		move_ = { 0.0f,0.0f,-0.05f };
	}

	if (worldTransform_.translation.z <= 0.0f)
	{
		move_ = { 0.0f,0.0f,0.05f };
	}

	worldTransform_.UpdateMatrix();
}

void MoveGround::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void MoveGround::OnCollision(Collider* collider)
{

}

Vector3 MoveGround::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
	return pos;
}