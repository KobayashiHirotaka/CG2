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
	
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	engine_ = MyEngine::GetInstance();

	uvChecker = engine_->LoadTexture("resources/uvChecker.png");
	monsterBall = engine_->LoadTexture("resources/monsterBall.png");
	modelData_ = engine_->LoadObjFile("resources", "plane.obj");

	audio_->soundDatas[0] = audio_->SoundLoadWave("resources/mokugyo.wav");

	triangleData[0] = { -0.5f,-0.5f,0.0f,1.0f };
	triangleData[1] = { 0.0f,0.5f,0.0f,1.0f };
	triangleData[2] = { 0.5f,-0.5f,0.0f,1.0f };
}

void GamePlayScene::Update()
{
	camera_->Update();

	if (input_->PushKey(DIK_SPACE))
	{
		count_ += 1;
	}

	if (input_->PushKey(DIK_RETURN))
	{
		audio_->Play(audio_->xAudio2.Get(), audio_->soundDatas[0]);
	}

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
}

void GamePlayScene::Draw()
{
	/*engine_->Draw(triangleData[0], triangleData[1], triangleData[2], material[0], camera_->transformationMatrixData, uvChecker);*/
	/*engine_->DrawSprite(LeftTop, LeftBottom, RightTop, RightBottom, material[0], uvChecker);
	engine_->DrawSphere(sphere, camera_->transformationMatrixData, material[0], sphereTexture);*/
	engine_->DrawModel(modelData_, { 0,0,0 }, camera_->transformationMatrixData, material[0]);

	ImGui::Begin("TriAngleColor");

	float color[3] = { material[0].x,material[0].y ,material[0].w };
	ImGui::SliderFloat3("RGB", color, 0, 1, "%.3f");
	ImGui::ColorEdit3("MaterialColor", color);
	material[0] = { color[0],color[1],color[2] };

	ImGui::End();

	ImGui::Begin("sphereTexture");
	ImGui::Checkbox("texture", &changeTexture);
	ImGui::End();

	ImGui::Begin("count");
	ImGui::Text("count %d", count_);
	ImGui::End();

	engine_->ImGui();

	if (changeTexture == true)
	{
		sphereTexture = monsterBall;

	}
	else {
		sphereTexture = uvChecker;
	}

	engine_->VertexReset();
}