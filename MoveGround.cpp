#include "MoveGround.h"
#include <cassert>

void MoveGround::Initialize(Model* model, Vector3 position)
{
	assert(model);
	worldTransform_.Initialize();
	model_ = model;
	worldTransform_.translation = position;

	move_ = { 0.0f,0.0f,0.05f };
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
