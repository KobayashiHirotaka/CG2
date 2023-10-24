#include "Goal.h"

void Goal::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation = { 0.0f,1.2f,55.0f };

	SetCollisionAttribute(kCollisionAttributeGoal);
	SetCollisionMask(kCollisionMaskGoal);
	SetCollisionPrimitive(kCollisionPrimitiveSphere);
}

void Goal::Update()
{
	worldTransform_.UpdateMatrix();
}

void Goal::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Goal::OnCollision(Collider* collider)
{
	isHit_ = true;
	ImGui::Begin("Goal");
	ImGui::Text("Clear");
	ImGui::End();
}

Vector3 Goal::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
	return pos;
}
