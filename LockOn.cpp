#include "LockOn.h"

void LockOn::Initialize()
{
	input_ = Input::GetInstance();

	textureManager_ = TextureManager::GetInstance();

	texture_ = textureManager_->LoadTexture("resource/reticle.png");
}

//void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection)
//{
//
//}

void LockOn::Draw()
{

}