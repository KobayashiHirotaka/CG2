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

	//天球
	skydomeModel_.reset(Model::CreateModelFromObj("resource/skydome", "skydome.obj"));

	player_ = std::make_unique<Player>();
	player_->Initialize(playerModel_.get());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());
}

void GamePlayScene::Update()
{
	player_->Update();

	skydome_->Updata();

	viewProjection_.UpdateMatrix();
	
	followCamera_->Update();
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();
}

void GamePlayScene::Draw()
{
	skydome_->Draw(viewProjection_);

	player_->Draw(viewProjection_);

	ImGui::Begin("count");
	ImGui::Text("count %d", count_);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("rotation", &viewProjection_.rotation.x, 1.0f, -1.0f);
	ImGui::SliderFloat3("transform", &viewProjection_.translation.x, 50.0f, -50.0f);
	ImGui::End();

	ImGui::Begin("Ground");
	ImGui::SliderFloat3("transform", &worldTransform_[0].rotation.x, 10.0f, -10.0f);
	ImGui::End();

	engine_->ImGui();
}