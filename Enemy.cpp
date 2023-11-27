#include "Enemy.h"
#include "Player.h"

void Enemy::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	startPosition_ = worldTransform_.translation;

	worldTransformHead_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformBody_.translation = { 0.0f,1.0f,0.0f };

	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation.x = 0.5f;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation.x = -0.5f;

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = & worldTransformBody_;

	FloatingGimmickInitialize();

	move_ = { 0.05f,0.0f,0.0f };

	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionMaskEnemy);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);
}

void Enemy::Update()
{
	preIsHit_ = isHit_;
	isHit_ = false;

	particles_.remove_if([](std::unique_ptr<Particle>& particle) {
		if (particle->GetIsAlive() == false) {
			particle.reset();
			return true;
		}
		return false;
		});

	if (isPlayerAttack_ == false)
	{
		hitCount_ = 0;
	}

	worldTransform_.translation = Add(worldTransform_.translation, move_);

	if (worldTransform_.translation.x >= 5.0f)
	{
		move_ = { -0.05f,0.0f,0.0f };
	}

	if (worldTransform_.translation.x <= -5.0f)
	{
		move_ = { 0.05f,0.0f,0.0f };
	}

	if (isDead_) 
	{
		const float kRotSpeed = 0.2f;
		const float kDisappearanceTime = 40.0f;

		worldTransform_.translation = Add(worldTransform_.translation, deathAnimationVelocity);

		worldTransform_.rotation.x += kRotSpeed;

		worldTransform_.scale.x -= 1.0f / kDisappearanceTime;
		worldTransform_.scale.y -= 1.0f / kDisappearanceTime;
		worldTransform_.scale.z -= 1.0f / kDisappearanceTime;

		if (worldTransform_.scale.x <= 0.0f || worldTransform_.scale.y <= 0.0f || worldTransform_.scale.z <= 0.0f) 
		{
			isDeathAnimationEnd_ = true;
		}
	}

	for (const std::unique_ptr<Particle>& particle : particles_)
	{
		particle->Update();
	}

	FloatingGimmickUpdate();
	ICharacter::Update();

	worldTransform_.UpdateMatrix(RotationType::Euler);

	worldTransformBody_.UpdateMatrix(RotationType::Euler);
	worldTransformHead_.UpdateMatrix(RotationType::Euler);
	worldTransformL_arm_.UpdateMatrix(RotationType::Euler);
	worldTransformR_arm_.UpdateMatrix(RotationType::Euler);

	ImGui::Begin("Hit");
	ImGui::Text("HitCount %d", hitCount_);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
	
	for (const std::unique_ptr<Particle>& particle : particles_)
	{
		particle->Draw(viewProjection);
	}
}

void Enemy::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
}

void Enemy::OnCollision(Collider* collider)
{
	if (collider->GetCollisionAttribute() & kCollisionAttributeWeapon)
	{
		isHit_ = true;

		Particle* particle = new Particle();
		particle->Initialize(models_[4], worldTransform_.translation);
		particles_.push_back(std::unique_ptr<Particle>(particle));

		if (isHit_ != preIsHit_)
		{
			hitCount_++;
		}

		if (hitCount_ >= Player::ComboNum) 
		{
			isDead_ = true;
			const float kSpeed = 0.5f;

			deathAnimationVelocity = { 0.0f,0.2f,kSpeed };
			deathAnimationVelocity = TransformNormal(deathAnimationVelocity, collider->GetWorldTransform().matWorld);
		}
	}
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
	return pos;
}

void Enemy::FloatingGimmickInitialize()
{
	for (int i = 0; i < kMaxModelParts; i++)
	{
		floatingParameter_[i] = 0.0f;
	}
}

void Enemy::FloatingGimmickUpdate()
{
	floatingCycle_[0] = 120;
	floatingCycle_[1] = 120;

	float step[2]{};

	for (int i = 0; i < kMaxModelParts; i++)
	{
		step[i] = 2.0f * (float)std::numbers::pi / floatingCycle_[i];

		floatingParameter_[i] += step[i];

		floatingParameter_[i] = (float)std::fmod(floatingParameter_[i], 2.0f * (float)std::numbers::pi);
	}

	worldTransformL_arm_.rotation.x = std::sin(floatingParameter_[1]) * 0.75f;
	worldTransformR_arm_.rotation.x = -std::sin(floatingParameter_[1]) * 0.75f;
}

Vector3 Enemy::GetCenterPosition() const 
{
	const Vector3 offset = { 0.0f,1.0f,0.0f };

	Vector3 worldPos = PositionTransform(offset, worldTransform_.matWorld);
	return worldPos;
}

void Enemy::ReStart()
{
	worldTransform_.translation = startPosition_;
	worldTransform_.rotation = { 0.0f,0.0f,0.0f };
	worldTransform_.scale = { 1.0f,1.0f,1.0f };
	isDead_ = false;
	isDeathAnimationEnd_ = false;
}