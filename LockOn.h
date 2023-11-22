#pragma once
#include "Input.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Enemy.h"

class LockOn
{
public:
	void Initialize();

	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	void Draw(const ViewProjection& viewProjection);

private:
	TextureManager* textureManager_ = nullptr;

	WorldTransform worldTransform_;

	std::unique_ptr<Sprite> lockOnMark_;

	const Enemy* target_ = nullptr;

	int texture_;

	Vector4 LeftTop_ = {360.0f,0.0f,0.0f,1.0f};
	Vector4 LeftBottom_ = { 360.0f,360.0f,0.0f,1.0f};
	Vector4 RightTop_ = { 640.0f,0.0f,0.0f,1.0f };
	Vector4 RightBottom_ = { 640.0f,360.0f,0.0f,1.0f };

	const float kDegreeToRadian_;

	float minDistance_ = 10.0f;

	float maxDistance_ = 30.0f;

	float angleRange_ = 20.0f * kDegreeToRadian_;

	Input* input_ = nullptr;

	XINPUT_STATE joyState_;
};

