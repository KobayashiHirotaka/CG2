#pragma once
#include "WindowsApp.h"
#include"DirectXCommon.h"
#include "TextureManager.h"

#include <fstream>
#include <numbers>

class MyEngine
{
public:
	static MyEngine* GetInstance();

	void Initialize();

	void ImGui();

private:
	//MyEngine() = default;
	//~MyEngine() = default;

	static const int kMaxTexture = 10;

	bool CheckTextureIndex[kMaxTexture];

	HRESULT hr_;
	WindowsApp* win_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	TextureManager* textureManager_ = nullptr;
};
