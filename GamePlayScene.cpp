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
	sprite_->Initialize(LeftTop[0], LeftBottom[0], RightTop[1], RightBottom[1]);
	worldTransform_Sprite_.Initialize();*/

	sphere_ = new Sphere();
	sphere_->Initialize();
	worldTransform_.Initialize();

	texture_ = textureManager_->LoadTexture("resource/uvChecker.png");

	audio_->soundDatas[0] = audio_->SoundLoadWave("resource/mokugyo.wav");

	/*model_ = model_->CreateModelFromObj("resource","plane.obj");*/

	worldTransform_.translation.z = -10.0f;

	viewProjection_.Initialize();
	/*worldTransform_Model_.Initialize();*/
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
		worldTransform_.translation.y += 0.05f;
	}

	if (input_->PressKey(DIK_S))
	{
		worldTransform_.translation.y -= 0.05f;
	}

	if (input_->PressKey(DIK_A))
	{
		worldTransform_.translation.x -= 0.05f;
	}

	if (input_->PressKey(DIK_D))
	{
		worldTransform_.translation.x += 0.05f;
	}

	viewProjection_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
	/*worldTransform_Model_.UpdateMatrix();*/
}

void GamePlayScene::Draw()
{
	sphere_->Draw(worldTransform_, viewProjection_, texture_);

	/*model_->Draw(worldTransform_Model_, viewProjection_);*/

	/*ImGui::Begin("TriAngleColor");

	float color[3] = { material[0].x,material[0].y ,material[0].w };
	ImGui::SliderFloat3("RGB", color, 0, 1, "%.3f");
	ImGui::ColorEdit3("MaterialColor", color);
	material[0] = { color[0],color[1],color[2] };

	ImGui::End();*/

	/*ImGui::Begin("sphereTexture");
	ImGui::Checkbox("texture", &changeTexture);
	ImGui::End();*/

	ImGui::Begin("count");
	ImGui::Text("count %d", count_);
	ImGui::End();

	engine_->ImGui();

	/*if (changeTexture == true)
	{
		sphereTexture = monsterBall;

	}
	else {
		sphereTexture = uvChecker;
	}

	engine_->VertexReset();*/
}