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

	worldTransform_.quaternion = Slerp(worldTransform_.quaternion, moveQuaternion_, 0.2f);
	worldTransform_.quaternion = Normalize(worldTransform_.quaternion);
	worldTransform_.UpdateMatrix(RotationType::Quaternion);
}

void LockOn::Draw(const ViewProjection& viewProjection)
{
	if (target_)
	{
		lockOnMark_->Draw(worldTransform_,texture_);
	}
}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection)
{
	std::list<std::pair<float, const Enemy*>> targets;

	for (const std::unique_ptr<Enemy>& enemy : enemies)
	{
		Vector3 positionWorld = enemy->GetWorldPosition();

		Vector3 positionView = TransformNormal(positionWorld, viewProjection.matView);

		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_)
		{
			Vector3 arcTangent = Normalize(arcTangent);
			Vector3 cross = Normalize(Cross({ 0.0f,0.0f,1.0f }, arcTangent));
			float dot = Dot({ 0.0f,0.0f,1.0f }, arcTangent);
			moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));

			/*if(arcTangent)*/
		}
	}
}