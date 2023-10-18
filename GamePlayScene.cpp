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

	model_ = model_->CreateModelFromObj("resource","plane.obj");

	model2_= model2_->CreateModelFromObj("resource", "plane.obj");

	//天球
	skydomeModel_.reset(Model::CreateModelFromObj("resource/skydome", "skydome.obj"));

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	/*worldTransform_[0].rotation.x =*/

	viewProjection_.Initialize();
	worldTransformModel_.Initialize();
	worldTransform_[0].Initialize();
}

void GamePlayScene::Update()
{
	camera_->Update();

	skydome_->Updata();

#ifdef _DEBUG
	if (input_->PushKey(DIK_1))
	{
		camera_->DebugCamera(true);
	}

	if (input_->PushKey(DIK_2))
	{
		camera_->DebugCamera(false);
	}
#endif // _DEBUG

	if (input_->PushKey(DIK_SPACE))
	{
		count_ += 1;
	}

	if (input_->PushKey(DIK_RETURN))
	{
		audio_->Play(audio_->xAudio2.Get(), audio_->soundDatas[0]);
	}

	if (input_->PressKey(DIK_W))
	{
		worldTransformModel_.translation.y += 0.05f;
	}

	if (input_->PressKey(DIK_S))
	{
		worldTransformModel_.translation.y -= 0.05f;
	}

	if (input_->PressKey(DIK_A))
	{
		worldTransformModel_.translation.x -= 0.05f;
	}

	if (input_->PressKey(DIK_D))
	{
		worldTransformModel_.translation.x += 0.05f;
	}

	if (input_->PressKey(DIK_Q))
	{
		worldTransformModel_.translation.z += 0.05f;
	}

	if (input_->PressKey(DIK_E))
	{
		worldTransformModel_.translation.z -= 0.05f;
	}


	viewProjection_.UpdateMatrix();
	worldTransformModel_.UpdateMatrix();
	worldTransform_[0].UpdateMatrix();
}

void GamePlayScene::Draw()
{
	model_->Draw(worldTransformModel_, viewProjection_);

	model2_->Draw(worldTransform_[0], viewProjection_);

	skydome_->Draw(viewProjection_);

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