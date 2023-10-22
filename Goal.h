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

	WorldTransform GetWorldTransform()override { return worldTransform_; }

	void OnCollision()override;

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
};

