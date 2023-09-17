#pragma once
#include "IScene.h"
#include "MyEngine.h"
#include "Camera.h"
#include "Sphere.h"
#include "ModelData.h"
#include "Input.h"

class GamePlayScene : public IScene
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	MyEngine* engine_ = nullptr;
	Camera* camera_ = nullptr;
	Input* input_ = nullptr;

	Vector4 triangleData[3] = {};
	Vector4 material[2] = { 1.0f,1.0f,1.0f,1.0f };

	Vector4 LeftTop = { 0.0f,0.0f,0.0f,1.0f };
	Vector4 LeftBottom = { 0.0f,360.0f,0.0f,1.0f };
	Vector4 RightTop = { 640.0f,0.0f,0.0f,1.0f };
	Vector4 RightBottom = { 640.0f,360.0f,0.0f,1.0f };

	Sphere sphere = { {0.0f,0.0f,0.0f},16 };

	int uvChecker;
	int monsterBall;

	int sphereTexture = 0;
	bool changeTexture = true;

	ModelData modelData_;

	int count_ = 0;
};
