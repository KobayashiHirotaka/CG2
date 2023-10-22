#include "GamePlayScene.h"

GamePlayScene::GamePlayScene()
{

}

GamePlayScene::~GamePlayScene()
{
	audio_->xAudio2.Reset();
	audio_->SoundUnload(&audio_->soundDatas[0]);
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

	audio_->soundDatas[0] = audio_->SoundLoadWave("resource/mokugyo.wav");

	viewProjection_.Initialize();

	playerModel_.reset(Model::CreateModelFromObj("resource/player", "player.obj"));

	goalModel_.reset(Model::CreateModelFromObj("resource/player", "player.obj"));

	skydomeModel_.reset(Model::CreateModelFromObj("resource/skydome", "skydome.obj"));

	collisionManager_ = std::make_unique<CollisionManager>();

	player_ = std::make_unique<Player>();
	player_->Initialize(playerModel_.get());

	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModel_.get());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	groundManager_ = std::make_unique<GroundManager>();
	groundManager_->Initialize();

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	followCamera_->SetTarget(&player_->GetPlayerWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());
}

void GamePlayScene::Update()
{
	player_->Update();

	goal_->Update();

	skydome_->Updata();

	groundManager_->Update();

	viewProjection_.UpdateMatrix();
	
	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();

	collisionManager_->ClearColliders();
	collisionManager_->AddCollider(player_.get());
	collisionManager_->AddCollider(goal_.get());

	collisionManager_->CheckAllCollision();
}

void GamePlayScene::Draw()
{
	player_->Draw(viewProjection_);

	goal_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);

	groundManager_->Draw(viewProjection_);
	
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("rotation", &viewProjection_.rotation.x, 1.0f, -1.0f);
	ImGui::SliderFloat3("transform", &viewProjection_.translation.x, 50.0f, -50.0f);
	ImGui::End();

	engine_->ImGui();
}