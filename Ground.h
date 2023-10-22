#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Ground
{
public:
	void Initialize(Model* model, Vector3 position);

	void Update();

	void Draw(ViewProjection& viewProjection);

	WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
};

