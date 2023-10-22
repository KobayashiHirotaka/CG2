#include "GroundManager.h"

void GroundManager::Initialize()
{
	groundModel_.reset(Model::CreateModelFromObj("resource/ground", "ground.obj"));
	moveGroundModel_.reset(Model::CreateModelFromObj("resource/ground", "ground.obj"));

	for (int i = 0; i < 2; i++)
	{
		ground_[i] = std::make_unique<Ground>();
	}

	moveGround_ = std::make_unique<MoveGround>();


	ground_[0]->Initialize(groundModel_.get(), {0.0f,0.0f,-28.0f});
	ground_[1]->Initialize(groundModel_.get(), { 0.0f,0.0f,48.0f });

	moveGround_->Initialize(moveGroundModel_.get(), { 0.0f,0.0f,10.0f });
}

void GroundManager::Update()
{
	for (int i = 0; i < 2; i++)
	{
		ground_[i]->Update();
	}

	moveGround_->Update();

	ImGui::Begin("Ground");
	ImGui::SliderFloat3("translation", &ground_[0]->GetWorldTransform().translation.x, -60.0f, 60.0f, "%.3f");
	ImGui::End();
}

void GroundManager::Draw(ViewProjection& viewProjection)
{
	for (int i = 0; i < 2; i++)
	{
		ground_[i]->Draw(viewProjection);
	}

	
	moveGround_->Draw(viewProjection);
}
