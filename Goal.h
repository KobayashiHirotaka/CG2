#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Goal : public Collider
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

	WorldTransform& GetWorldTransform()override { return worldTransform_; }

	Vector3 GetWorldPosition() override;

	void OnCollision(Collider* collider)override;

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
};

