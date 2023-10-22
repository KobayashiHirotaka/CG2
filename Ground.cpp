#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model, Vector3 position)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation = position;
}

void Ground::Update()
{
	worldTransform_.UpdateMatrix();
}

void Ground::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
