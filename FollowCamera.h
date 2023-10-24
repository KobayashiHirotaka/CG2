#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "MyMath.h"

class FollowCamera
{
public:
	void Initialize();

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void SetTarget(const WorldTransform* target) { target_ = target; }

	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	const WorldTransform* target_ = nullptr;

	ViewProjection viewProjection_;

	Input* input_ = nullptr;

	XINPUT_STATE joyState_;
};

