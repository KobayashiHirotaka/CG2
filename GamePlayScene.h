#pragma once
#include "IScene.h"
#include "MyEngine.h"
#include "Camera.h"
#include "Light.h"
#include "Triangle.h"
#include "Sprite.h"
#include "Sphere.h"
#include "Model.h"
#include "ModelData.h"
#include "Input.h"
#include "Audio.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"

#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class GamePlayScene : public IScene
{
public:
	GamePlayScene();
	~GamePlayScene();

	void Initialize();
	void Update();
	void Draw();

private:
	MyEngine* engine_ = nullptr;

	Camera* camera_ = nullptr;

	TextureManager* textureManager_ = nullptr;

	Light* light_ = nullptr;

	Input* input_ = nullptr;

	Audio* audio_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	Model* model_ = nullptr;

	WorldTransform worldTransform_[2];

	WorldTransform worldTransformSprite_;

	WorldTransform worldTransformModel_;

	ViewProjection viewProjection_;

	Sprite* sprite_;

	Sphere* sphere_[2];
	
	int texture_;

	bool changeTexture_;

	int monsterBall_;

	int uvChecker_;

	uint32_t sound_;
	
	Vector4 LeftTop_[2] = {
		{ 0.0f,0.0f,0.0f,1.0f },
		{ 360.0f,0.0f,0.0f,1.0f }
	};
	Vector4 LeftBottom_[2] = {
		{ 0.0f,360.0f,0.0f,1.0f },
		{ 360.0f,360.0f,0.0f,1.0f }
	};
	Vector4 RightTop_[2] = {
		{ 360.0f,0.0f,0.0f,1.0f },
		{ 640.0f,0.0f,0.0f,1.0f }
	};
	Vector4 RightBottom_[2] = {
		{ 360.0f,180.0f,0.0f,1.0f },
		{ 640.0f,360.0f,0.0f,1.0f }
	};

	int count_ = 0;
};
