#include "LockOn.h"
#include "Enemy.h"

void LockOn::Initialize()
{
	input_ = Input::GetInstance();

	textureManager_ = TextureManager::GetInstance();

	texture_ = textureManager_->LoadTexture("resource/reticle.png");

	lockOnMark_ = std::make_unique<Sprite>();
	lockOnMark_->Initialize(LeftTop_[0], LeftBottom_[0], RightTop_[1], RightBottom_[1]);

	worldTransformReticle_.Initialize();
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection)
{
	if (isLockOn_)
	{
		if (target_)
		{
			if (input_->IsPressButtonEnter(XINPUT_GAMEPAD_RIGHT_THUMB))
			{
				target_ = nullptr;

			}
			else if (InRange(viewProjection)) {
				target_ = nullptr;

			}
			else if (target_->GetIsDead()) {
				target_ = nullptr;
			}

			if (input_->IsPressButtonEnter(XINPUT_GAMEPAD_X))
			{
				SearchLockOnTarget(enemies, viewProjection);
				targetIndex_++;

				if (targetIndex_ >= targets_.size())
				{
					targetIndex_ = 0;
					target_ = nullptr;
					target_ = targets_[targetIndex_].second;

				}
				else {
					target_ = nullptr;
					target_ = targets_[targetIndex_].second;
				}
			}

		}
		else {
			if (input_->IsPressButtonEnter(XINPUT_GAMEPAD_RIGHT_THUMB))
			{
				SearchLockOnTarget(enemies, viewProjection);
			}
		}

	} else {
		if (input_->IsPressButtonEnter(XINPUT_GAMEPAD_RIGHT_THUMB))
		{
			if (isAutoLockOn_)
			{
				isAutoLockOn_ = false;
				target_ = nullptr;

			}
			else {
				isAutoLockOn_ = true;
			}
		}

		if (target_ && InRange(viewProjection))
		{
			target_ = nullptr;

		}
		else if (target_ && target_->GetIsDead())
		{
			target_ = nullptr;
		}

		if (isAutoLockOn_)
		{
			SearchLockOnTarget(enemies, viewProjection);
		}
	}
	

	if (target_) {
		
		Vector3 positionWorld = target_->GetCenterPosition();
	
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WindowsApp::kClientWidth, WindowsApp::kClientHeight, 0, 1);
		
		Matrix4x4 matViewProjectionViewport = Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));

		Vector3 positionScreen = PositionTransform(positionWorld, matViewProjectionViewport);

		worldTransformReticle_.translation.x = positionScreen.x;

		worldTransformReticle_.translation.y = positionScreen.y;
	
		Vector2 positionScreenV2 = { positionScreen.x,positionScreen.y };
	}

	worldTransformReticle_.UpdateMatrix(RotationType::Euler);

	ImGui::Begin("LockOn");
	ImGui::Checkbox("IsLockOn", &isLockOn_);
	ImGui::Checkbox("IsAutoLockOn", &isAutoLockOn_);
	ImGui::End();
}

void LockOn::Draw()
{
	if (target_)
	{
		lockOnMark_->Draw(worldTransformReticle_, texture_);
	}
}

void LockOn::SearchLockOnTarget(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) 
{
	targets_.clear();

	for (const std::unique_ptr<Enemy>& enemy : enemies)
	{
		Vector3 positionWorld = enemy->GetCenterPosition();
	
		Vector3 positionView = PositionTransform(positionWorld, viewProjection.matView);

		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_)
		{
			float norm = Length(positionView);
			float arcTangent = std::acos(positionView.z / norm);


			if (std::abs(arcTangent) <= angleRange_ && enemy->GetIsDead() == false) {
				targets_.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}
		}
	}

	target_ = nullptr;

	if (targets_.size() != 0) 
	{
		std::sort(targets_.begin(), targets_.end(), [](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
		
		target_ = targets_.front().second;
	}
}

bool LockOn::InRange(const ViewProjection& viewProjection)
{
	Vector3 targetPosition = target_->GetCenterPosition();

	Vector3 positionView = PositionTransform(targetPosition, viewProjection.matView);

	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
	
		float arcTangent = std::atan2(std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

		if (std::abs(arcTangent) <= angleRange_) 
		{
			return false;
		}
	}

	return true;
}

Vector3 LockOn::GetTargetPosition() const 
{
	if (target_)
	{
		return target_->GetCenterPosition();
	}

	return Vector3();
}