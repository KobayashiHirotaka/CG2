#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"

class Player : public Collider
{
public:

	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

	WorldTransform& GetWorldTransform()override { return worldTransform_; }

	Vector3 GetWorldPosition() override;

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	void OnCollision(Collider* collider)override;

private:

	WorldTransform worldTransform_;
	const ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	Input* input_ = nullptr;

	XINPUT_STATE joyState_;
};

