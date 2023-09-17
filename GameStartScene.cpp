#include "GameStartScene.h"

void GameStartScene::Initialize()
{

}

void GameStartScene::Update()
{
	time_++;
	if (time_ >= 60)
	{
		stateNum = 1;
	}

	ImGui::Begin("StateNum");
	ImGui::Text("StateNum %d", stateNum);
	ImGui::InputInt("SceneNum", &stateNum);
	ImGui::End();
}

void GameStartScene::Draw()
{

}
