#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"

#include "Ground.h"
#include "MoveGround.h"

class GroundManager
{
public:
	void Initialize();

	void Update();

	void Draw(ViewProjection& viewProjection);

	Ground* GetGround(int array) { return ground_->get(); }
	MoveGround* GetMoveGround() { return moveGround_.get(); }

private:
	std::unique_ptr<Ground> ground_[2];
	std::unique_ptr<Model> groundModel_;

	std::unique_ptr<MoveGround> moveGround_;
	std::unique_ptr<Model> moveGroundModel_;
};

