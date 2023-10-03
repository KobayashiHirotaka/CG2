#pragma once
#include "WindowsApp.h"
#include "DirectXCommon.h"
#include "MyEngine.h"
#include "MyImGui.h"
#include "Input.h"
#include "Audio.h"
#include "IScene.h"
#include "GameStartScene.h"
#include "GamePlayScene.h"
#include "GameEndScene.h"

class SceneManager
{
public:
	void Initialize();

	void UpDate();

	//void Draw();

private:
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	WindowsApp* win_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	MyEngine* engine_ = nullptr;
	MyImGui* imGui_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	MSG msg{};

	enum GameStateNo
	{
		START,
		PLAY,
		END
	};

	std::unique_ptr<IScene>state[END];

	void Release();
};