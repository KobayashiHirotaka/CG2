#include "LockOn.h"

void LockOn::Initialize()
{
	textureManager_ = TextureManager::GetInstance();

	lockOnMark_= std::make_unique<Sprite>();
	lockOnMark_->Initialize(LeftTop_, LeftBottom_, RightTop_, RightBottom_);

	worldTransform_.Initialize();

	texture_ = textureManager_->LoadTexture("resource/monsterBall.png");
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection)
{
	if (target_)
	{


	} else {
		if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{

		}
	}
}

void LockOn::Draw(const ViewProjection& viewProjection)
{
	if (target_)
	{
		lockOnMark_->Draw(worldTransform_,texture_);
	}
}