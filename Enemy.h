#include "Collider.h"
#include "CollisionConfig.h"
#include "ICharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"
#include <numbers>

class Enemy : public Collider, public ICharacter
{
public:
	void Initialize(const std::vector<Model*>& models)override;

	void Update()override;

	void Draw(const ViewProjection& viewProjection)override;

	WorldTransform& GetWorldTransform()override { return worldTransform_; }
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }

	Vector3 GetWorldPosition() override;

	void FloatingGimmickInitialize();

	void FloatingGimmickUpdate();

	void SetParent(const WorldTransform* parent);

	void OnCollision(Collider* collider)override;

	void SetPosition(const Vector3& position) { worldTransform_.translation = position; };

	Vector3 GetCenterPosition() const;

	bool GetIsDead() const { return isDead_; };

	bool GetIsDeathAnimationEnd() const { return isDeathAnimationEnd_; };

	void SetIsPlayerAttack(bool isPlayerAttack) { isPlayerAttack_ = isPlayerAttack; };

	void ReStart();

private:
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	const uint16_t kMaxModelParts = 2;

	Vector3 startPosition_;

	Vector3 move_;

	float floatingParameter_[2];

	int floatingCycle_[2];

	float floatingAmplitude_;

	uint32_t hitCount_ = 0;
	bool isPlayerAttack_ = false;

	bool isHit_ = false;
	bool preIsHit_ = false;

	bool isDead_ = false;
	bool isDeathAnimationEnd_ = false;

	Vector3 deathAnimationVelocity{ 0.0f,0.0f,0.0f };
};
