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

	/*sprite_ = new Sprite();
	sprite_->Initialize(LeftTop_[0], LeftBottom_[0], RightTop_[1], RightBottom_[1]);
	worldTransformSprite_.Initialize();*/

	for (int i = 0; i < 2; i++ )
	{
		sphere_[i] = new Sphere();
		sphere_[i]->Initialize();
		worldTransform_[i].Initialize();
	}

	uvChecker_ = textureManager_->LoadTexture("resource/uvChecker.png");
	monsterBall_ = textureManager_->LoadTexture("resource/monsterBall.png");

	texture_ = monsterBall_;

	audio_->soundDatas[0] = audio_->SoundLoadWave("resource/mokugyo.wav");

	/*model_ = model_->CreateModelFromObj("resource","plane.obj");*/

	worldTransform_[0].translation.x = 5.0f;
	worldTransform_[1].translation.z = -10.0f;

	viewProjection_.Initialize();
	/*worldTransformModel_.Initialize();*/
}

void GamePlayScene::Update()
{
	camera_->Update();

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
		worldTransform_[0].translation.y += 0.05f;
	}

	if (input_->PressKey(DIK_S))
	{
		worldTransform_[0].translation.y -= 0.05f;
	}

	if (input_->PressKey(DIK_A))
	{
		worldTransform_[0].translation.x -= 0.05f;
	}

	if (input_->PressKey(DIK_D))
	{
		worldTransform_[0].translation.x += 0.05f;
	}

	if (input_->PressKey(DIK_Q))
	{
		worldTransform_[0].translation.z += 0.05f;
	}

	if (input_->PressKey(DIK_E))
	{
		worldTransform_[0].translation.z -= 0.05f;
	}


	if (input_->PressKey(DIK_UP))
	{
		worldTransform_[0].rotation.x += 0.05f;
	}

	if (input_->PressKey(DIK_DOWN))
	{
		worldTransform_[0].rotation.x -= 0.05f;
	}

	if (input_->PressKey(DIK_LEFT))
	{
		worldTransform_[0].rotation.y += 0.05f;
	}

	if (input_->PressKey(DIK_RIGHT))
	{
		worldTransform_[0].rotation.y -= 0.05f;
	}

	viewProjection_.UpdateMatrix();
	worldTransform_[0].UpdateMatrix();
	/*worldTransformModel_.UpdateMatrix();*/
}

void GamePlayScene::Draw()
{
	sphere_[0]->Draw(worldTransform_[0], viewProjection_, texture_);
	sphere_[1]->Draw(worldTransform_[1], viewProjection_, texture_);

	/*model_->Draw(worldTransformModel_, viewProjection_);*/

	ImGui::Begin("sphereTexture");
	ImGui::Checkbox("texture", &changeTexture_);
	ImGui::End();

	ImGui::Begin("count");
	ImGui::Text("count %d", count_);
	ImGui::End();

	engine_->ImGui();

	if (changeTexture_ == true)
	{
		texture_ = monsterBall_;

	}else {
		texture_ = uvChecker_;
	}
}