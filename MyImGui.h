#pragma once
#include "WindowsApp.h"
#include "DirectXCommon.h"

class MyImGui
{
public:
	void Initialize(WindowsApp* win, DirectXCommon* dxCommon);

	void BeginFlame();

	void EndFlame();
};

