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

private:
	Model* model_;

	WorldTransform worldTransformParticle_;

	Vector3 speed_;

	int particleTimer = 150;

	bool isAlive = false;
};

