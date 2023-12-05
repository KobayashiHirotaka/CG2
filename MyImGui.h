#pragma once
#include "Engine/Base/WindowsApp/WindowsApp.h"
#include "Engine/Base/DirectXCore/DirectXCore.h"

class MyImGui
{
public:
	static MyImGui* GetInstance();

	//MyImGui();
	//~MyImGui();

	void Initialize(WindowsApp* win, DirectXCore* dxCore);

	void BeginFlame();

	void EndFlame();
};

