#pragma once
#include "IScene.h"
#include"MyImGui.h"

class GameStartScene : public IScene
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	int time_;
};
