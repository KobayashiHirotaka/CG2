#include "Particle.h"

void Particle::Intialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);
}

void Particle::Update()
{
	ICharacter::Update();
}

void Particle::Draw(const ViewProjection& viewProjection)
{
	models_[0]->Draw(worldTransform_, viewProjection);
}
