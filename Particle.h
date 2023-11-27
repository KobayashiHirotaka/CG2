#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

#include "ICharacter.h"

class Particle : public ICharacter
{
public:
	void Intialize(const std::vector<Model*>& models);

	void Update();

	void Draw(const ViewProjection& viewProjection);
};

