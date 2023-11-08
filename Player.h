#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "ICharacter.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"
#include "Weapon.h"
#include <optional>

class Player : public Collider, public ICharacter
{
public:
	void Initialize(const std::vector<Model*>& models)override;

	void Update()override;

	void Draw(const ViewProjection& viewProjection)override;

	WorldTransform& GetWorldTransform()override { return worldTransform_; }

	Vector3 GetWorldPosition() override;

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	void OnCollision(Collider* collider)override;

	void SetParent(const WorldTransform* parent) { parent_ = parent; };

	void Restart();

	void ApplyGlobalVariables();

	void BehaviorRootInitialize();

	void BehaviorRootUpdate();

	void BehaviorAttackInitialize();

	void BehaviorAttackUpdate();

	void BehaviorDashInitialize();

	void BehaviorDashUpdate();

	Weapon* GetWeapon() { return weapon_.get(); };

private:
	enum class Behavior
	{
		kRoot,
		kAttack,
		kDash
	};

	struct WorkDash
	{
		int dashParameter_= 0;
		int coolTime = 0;
	};

	const WorldTransform* parent_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

	Input* input_ = nullptr;

	XINPUT_STATE joyState_;

	float playerSpeed_ = 0.3f;

	float targetAngle_ = 0.0f;

	Behavior behavior_ = Behavior::kRoot;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	WorkDash workDash_;

	std::unique_ptr<Weapon> weapon_ = nullptr;

	int behaviorDashTime_ = 10;

	bool isHit_ = false;
	bool preIsHit_ = false;

	bool reStart_ = false;
};

