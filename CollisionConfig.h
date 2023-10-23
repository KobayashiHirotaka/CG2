#pragma once
#include <cstdint>

namespace CollisionConfig
{
	const uint32_t kCollisionAttributePlayer = 0b0001;
	const uint32_t kCollisionAttributeEnemy = 0b0010;
	const uint32_t kCollisionAttributeGround = 0b0100;
	const uint32_t kCollisionAttributeGoal = 0b1000;
};