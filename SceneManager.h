#pragma once
#include "Engine/Base/WindowsApp/WindowsApp.h"
#include "Engine/Base/DirectXCore/DirectXCore.h"
#include "Engine/Base/MyEngine/MyEngine.h"
#include "TextureManager.h"
#include "MyImGui.h"
#include "Input.h"
#include "Audio.h"
#include "Light.h"
#include "IScene.h"
#include "GameStartScene.h"
#include "GamePlayScene.h"
#include "GameEndScene.h"

class SceneManager
{
public:
	void Initialize();

	void Update();

	//void Draw();

	void Release();

	void Run();

	enum GameSceneNum
	{
		START,
		PLAY,
		END
	};

private:
	WindowsApp* win_ = nullptr;

	DirectXCore* dxCore_ = nullptr;

	MyEngine* engine_ = nullptr;

	MyImGui* imGui_ = nullptr;

	TextureManager* textureManager_ = nullptr;

	Input* input_ = nullptr;

	Audio* audio_ = nullptr;

	Light* light_ = nullptr;

	MSG msg{};

	std::unique_ptr<IScene>state[END];
};