#pragma once
#include "Sprite.h"
#include "Input.h"
#include "TextureManager.h"
#include "ViewProjection.h"

class Enemy;

class LockOn 
{
public:
	void Initialize();

	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	void Draw();

	void SearchLockOnTarget(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	bool InRange(const ViewProjection& viewProjection);

	Vector3 GetTargetPosition() const;

	bool ExistTarget() const { return target_ ? true : false; };

	void AutoLockOn(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

private:
	Input* input_ = nullptr;

	TextureManager* textureManager_ = nullptr;

	std::unique_ptr<Sprite> lockOnMark_;

	WorldTransform worldTransformReticle_;

	int texture_;

	const Enemy* target_ = nullptr;

	std::vector<std::pair<float, const Enemy*>> targets_;

	float minDistance_ = 10.0f;

	float maxDistance_ = 50.0f;

	const float kDegreeToRadian = 3.14159265358979323846f / 180.0f;

	float angleRange_ = 20.0f * kDegreeToRadian;

	bool isLockOn_ = true;

	bool isAutoLockOn_ = false;

	uint32_t targetIndex_ = 0;

	Vector4 LeftTop_[2] = {
		{ 0.0f,0.0f,0.0f,1.0f },
		{ 360.0f,0.0f,0.0f,1.0f }
	};
	Vector4 LeftBottom_[2] = {
		{ 0.0f,45.0f,0.0f,1.0f },
		{ 360.0f,360.0f,0.0f,1.0f }
	};
	Vector4 RightTop_[2] = {
		{ 360.0f,0.0f,0.0f,1.0f },
		{ 45.0f,0.0f,0.0f,1.0f }
	};
	Vector4 RightBottom_[2] = {
		{ 360.0f,180.0f,0.0f,1.0f },
		{ 45.0f,45.0f,0.0f,1.0f }
	};
};


