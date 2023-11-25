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
	enum class Behavior
	{
		kRoot,
		kAttack,
		kDash,
		kJump
	};

	struct WorkDash
	{
		int dashParameter = 0;
		int coolTime = 0;
	};

	struct WorkAttack
	{
		int attackParameter = 0;
		int comboIndex = 0;
		int inComboPhase = 0;
		bool comboNext = false;
	};

	struct ConstAttack
	{
		uint32_t anticipationTime;

		uint32_t chargeTime;

		uint32_t swingTime;

		uint32_t recoveryTime;

		float anticipationSpeed;

		float chargeSpeed;

		float swingSpeed;
	};

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

	void BehaviorJumpInitialize();

	void BehaviorJumpUpdate();

	Weapon* GetWeapon() { return weapon_.get(); };

private:
	const WorldTransform* parent_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

	Input* input_ = nullptr;

	XINPUT_STATE joyState_;

	float playerSpeed_ = 0.3f;

	Quaternion moveQuaternion_ = { 0.0f,0.0f,0.0f,1.0f };

	Behavior behavior_ = Behavior::kRoot;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	WorkDash workDash_;

	std::unique_ptr<Weapon> weapon_ = nullptr;

	int behaviorDashTime_ = 10;

	Vector3 velocity_ = {};

	static const int ComboNum = 3;

	WorkAttack workAttack_;

	static const std::array<ConstAttack, Player::ComboNum>kConstAttacks_;

	bool isHit_ = false;
	bool preIsHit_ = false;

	bool reStart_ = false;
};

