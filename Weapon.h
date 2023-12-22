#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <numbers>

class Weapon : public Collider
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	WorldTransform& GetWorldTransform() override { return worldTransform_; };

	Vector3 GetWorldPosition() override;

	void OnCollision(Collider* collider) override;

	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };

	void Attack();

	bool GetIsAttack() { return isAttack_; };

	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; };

	void SetTranslation(const Vector3& translation) { worldTransform_.translation = translation; };

	void SetRotation(const Vector3& rotation) { worldTransform_.rotation = rotation; }

private:
	void AttackInitialize();

	Model* model_ = nullptr;

	WorldTransform worldTransform_;
	WorldTransform collisionWorldTransform_;

	bool isHit_ = false;

	bool isAttack_ = false;
};

