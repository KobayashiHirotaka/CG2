#pragma once
#include "IScene.h"
#include "MyImGui.h"

class GameStartScene : public IScene
{
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
};
