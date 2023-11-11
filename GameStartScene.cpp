#include "GameStartScene.h"

void GameStartScene::Initialize()
{
	
}

void GameStartScene::Update()
{
	Quaternion rotation = MakeRotateAxisAngleQuaternion(Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	
	Vector3 pointY = { 2.1f,-0.9f,1.3f };

	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);

	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = TransformVectorByMatrix(pointY, rotateMatrix);
	

	ImGui::Begin("Quaternion");
	
	ImGui::Text("%1.2f,%1.2f,%1.2f,%1.2f      : rotation", rotation.x, rotation.y, rotation.z, rotation.w);

	ImGui::Text("rotateMatrix\n%1.3f %1.3f% 1.3f %1.3f\n%1.3f% 1.3f %1.3f %1.3f\n%1.3f %1.3f %1.3f %1.3f\n%1.3f %1.3f %1.3f %1.3f",
		rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3],
		rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3], 
		rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3], 
		rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);

	ImGui::Text("%1.2f,%1.2f,%1.2f      : rotateByQuaternion", rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z);
	ImGui::Text("%1.2f,%1.2f,%1.2f      : rotateByMatrix", rotateByMatrix.x, rotateByMatrix.y, rotateByMatrix.z);
	
	ImGui::End();

	/*ImGui::Begin("SceneNum");
	ImGui::Text("SceneNum %d", sceneNum);
	ImGui::InputInt("SceneNum", &sceneNum);
	ImGui::End();*/
}

void GameStartScene::Draw()
{

}
