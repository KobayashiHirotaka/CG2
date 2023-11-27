#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Particle
{
public:
	void Initialize(Model* model, Vector3 position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	float RandomSpeed();

	bool GetIsAlive() { return isAlive_; };

	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; };

private:
	Model* model_;

	WorldTransform worldTransformParticle_;

	Vector3 speed_;

	int particleTimer_ = 150;

	bool isAlive_ = false;
};

