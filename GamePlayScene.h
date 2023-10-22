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

#include "Player.h"
#include "FollowCamera.h"
#include "GroundManager.h"
#include "Skydome.h"

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

	ViewProjection viewProjection_;

	uint32_t sound_;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;

	std::unique_ptr<FollowCamera> followCamera_;

	/*std::unique_ptr<Ground> ground_;
	std::unique_ptr<Model> groundModel_;*/

	std::unique_ptr<GroundManager> groundManager_;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	bool isDebugCameraActive_ = false;
};
