#include "Goal.h"

void Goal::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation = { -13.0f,1.0f,18.0f };
	SetCollisionAttribute(CollisionConfig::kCollisionAttributeEnemy);
	SetCollisionMask(~CollisionConfig::kCollisionAttributeEnemy);
}

void Goal::Update()
{
	worldTransform_.UpdateMatrix();
}

void Goal::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Goal::OnCollision()
{
	ImGui::Begin("Goal");
	ImGui::Text("Clear!");
	ImGui::End();
}
