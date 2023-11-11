#include "GameStartScene.h"

void GameStartScene::Initialize()
{
	
}

void GameStartScene::Update()
{
	Vector3 axis = Normalize({ 1.0f,1.0f,1.0f });
	float angle = 0.44f;

	Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);

	ImGui::Begin("rotateMatrix");
	ImGui::Text("%1.3f %1.3f %1.3f %1.3f\n%1.3f %1.3f %1.3f %1.3f\n%1.3f %1.3f %1.3f %1.3f\n%1.3f %1.3f %1.3f %1.3f",
		rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3],
		rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3],
		rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3],
		rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);
	
	ImGui::End();

	/*ImGui::Begin("SceneNum");
	ImGui::Text("SceneNum %d", sceneNum);
	ImGui::InputInt("SceneNum", &sceneNum);
	ImGui::End();*/
}

void GameStartScene::Draw()
{

}
