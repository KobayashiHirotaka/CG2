#include "GameStartState.h"

void GameStartState::Initialize()
{

}

void GameStartState::Update()
{
	/*time_++;
	if (time_ >= 60)
	{
		stateNum = 1;
	}*/

	ImGui::Begin("StateNum");
	ImGui::Text("StateNum %d", stateNum);
	ImGui::InputInt("SceneNum", &stateNum);
	ImGui::End();
}

void GameStartState::Draw()
{

}
