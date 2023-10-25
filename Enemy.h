#include "Collider.h"
#include "CollisionConfig.h"
#include "ICharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Enemy : public Collider, ICharacter
{
public:
	void Initialize(const std::vector<Model*>& models)override;

	void Update()override;

	void Draw(const ViewProjection& viewProjection)override;

	WorldTransform& GetWorldTransform()override { return worldTransform_; }

	Vector3 GetWorldPosition() override;

	void OnCollision(Collider* collider)override;

	bool GetIsHit() { return isHit_; }

private:
	Vector3 move_;

	bool isHit_ = false;
};
