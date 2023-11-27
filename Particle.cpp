#include "Particle.h"

void Particle::Initialize(Model* model, Vector3 position)
{
	assert(model);
	model_ = model;

	worldTransformParticle_.Initialize();

	worldTransformParticle_.translation = position;
}

void Particle::Update()
{
	if (isAlive == false)
	{
		isAlive = true;

		speed_.x = RandomSpeed();
		speed_.y = RandomSpeed();
		speed_.z = RandomSpeed();
	}

	if (isAlive == true)
	{
		particleTimer--;
		worldTransformParticle_.translation.x += speed_.x;
		worldTransformParticle_.translation.y += speed_.y;
		worldTransformParticle_.translation.z += speed_.z;
		worldTransformParticle_.rotation.x += speed_.x;
		worldTransformParticle_.rotation.y += speed_.y;
		worldTransformParticle_.UpdateMatrix(RotationType::Euler);

		if (particleTimer < 0)
		{
			isAlive = false;
			particleTimer = 150;
		}
	}
	
}

void Particle::Draw(const ViewProjection& viewProjection)
{
	if (isAlive == true)
	{
		model_->Draw(worldTransformParticle_, viewProjection);
	}
}

float Particle::RandomSpeed()
{
	float min = -1.3f;
	float max = 1.3f;

	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}
