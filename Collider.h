#pragma once
#include "MyMath.h"
#include "WorldTransform.h"
#include <algorithm>

class Collider
{
private:
	float radius_ = 1.0f;
	uint32_t collisionAttribute_ = 0xffffffff;
	uint32_t CollisionMask_ = 0xffffffff;

public:
	float Getradius() const { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }
	virtual ~Collider() {}
	virtual void OnCollision(Collider* collider) = 0;
	virtual Vector3 GetWorldPosition() = 0;
	virtual WorldTransform& GetWorldTransform() = 0;
	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }
	void SetCollisionAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }
	uint32_t GetCollisionMask() const { return CollisionMask_; }
	void SetCollisionMask(uint32_t mask) { CollisionMask_ = mask; }
};

