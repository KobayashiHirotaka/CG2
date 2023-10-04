#include "GameStartScene.h"

void GameStartScene::Initialize()
{
	
}

void GameStartScene::Update()
{
	ImGui::Begin("StateNum");
	ImGui::Text("StateNum %d", stateNum);
	ImGui::InputInt("SceneNum", &stateNum);
	ImGui::End();
}

void GameStartScene::Draw()
{

}
