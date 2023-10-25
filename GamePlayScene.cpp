#include "GamePlayScene.h"

GamePlayScene::GamePlayScene()
{

}

GamePlayScene::~GamePlayScene()
{

}

void GamePlayScene::Initialize()
{
	camera_ = new Camera();
	camera_->Initialize(1280, 720);

	dxCommon_ = DirectXCommon::GetInstance();
	
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureManager_ = TextureManager::GetInstance();

	light_ = Light::GetInstance();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	playerModel_.reset(Model::CreateModelFromObj("resource/player", "player.obj"));

	enemyModel_.reset(Model::CreateModelFromObj("resource/goal", "goal.obj"));

	goalModel_.reset(Model::CreateModelFromObj("resource/goal", "goal.obj"));

	groundModel_.reset(Model::CreateModelFromObj("resource/ground", "ground.obj"));

	skydomeModel_.reset(Model::CreateModelFromObj("resource/skydome", "skydome.obj"));

	collisionManager_ = std::make_unique<CollisionManager>();

	std::vector<Model*> playerModels = { playerModel_.get()};

	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(goalModel_.get());

	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModel_.get());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());


	for (int i = 0; i < 2; i++)
	{
		ground_[i] = std::make_unique<Ground>();
	}

	ground_[0]->Initialize(groundModel_.get(), { 0.0f,0.0f,0.0f });
	ground_[1]->Initialize(groundModel_.get(), { 0.0f,0.0f,68.0f });

	moveGround_ = std::make_unique<MoveGround>();

	moveGround_->Initialize(groundModel_.get(), { 0.0f,0.0f,30.0f });

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&viewProjection_);
}

void GamePlayScene::Update()
{
	if (goal_->GetIsHit() == true || enemy_->GetIsHit() == true)
	{
		Initialize();
	}

	player_->Update();

	enemy_->Update();

	goal_->Update();

	skydome_->Updata();

	for (int i = 0; i < 2; i++)
	{
		ground_[i]->Update();
	}

	moveGround_->Update();

	viewProjection_.UpdateMatrix();
	
	followCamera_->Update();
	viewProjection_ = followCamera_->GetViewProjection();

	collisionManager_->ClearColliders();
	collisionManager_->AddCollider(player_.get());
	collisionManager_->AddCollider(enemy_.get());

	for (int i = 0; i < 2; i++)
	{
		collisionManager_->AddCollider(ground_[i].get());
	}

	collisionManager_->AddCollider(moveGround_.get());
	collisionManager_->AddCollider(goal_.get());
	collisionManager_->CheckAllCollision();
}

void GamePlayScene::Draw()
{
	player_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);

	goal_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);

	for (int i = 0; i < 2; i++)
	{
		ground_[i]->Draw(viewProjection_);
	}

	moveGround_->Draw(viewProjection_);

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("rotation", &viewProjection_.rotation.x, 1.0f, -1.0f);
	ImGui::SliderFloat3("transform", &viewProjection_.translation.x, 50.0f, -50.0f);
	ImGui::End();

	engine_->ImGui();
}