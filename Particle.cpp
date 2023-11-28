#include "Particle.h"

void Particle::Initialize(Model* model, Vector3 position)
{
	assert(model);
	model_ = model;

	isAlive_ = true;

	worldTransformParticle_.Initialize();

	worldTransformParticle_.translation = position;

	speed_.x = RandomSpeed();
	speed_.y = RandomSpeed();
	speed_.z = RandomSpeed();
}

void Particle::Update()
{	
	if (isAlive_ == true)
	{
		particleTimer_--;
		worldTransformParticle_.translation.x += speed_.x;
		worldTransformParticle_.translation.y += speed_.y;
		worldTransformParticle_.translation.z += speed_.z;
		worldTransformParticle_.rotation.x += speed_.x;
		worldTransformParticle_.rotation.y += speed_.y;
		worldTransformParticle_.scale.x -= 0.01f;
		worldTransformParticle_.scale.y -= 0.01f;
		worldTransformParticle_.scale.z -= 0.01f;
		worldTransformParticle_.UpdateMatrix(RotationType::Euler);
	}

	if (particleTimer_ < 0)
	{
		isAlive_ = false;
	}
}

void Particle::Draw(const ViewProjection& viewProjection)
{
	if (isAlive_ == true)
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
