#include<Windows.h>
#include"WindowsApp.h"
#include"DirectXCommon.h"
#include"MyEngine.h"
#include"Triangle.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WindowsApp* winApp = new WindowsApp;
	DirectXCommon* directXCommon = new DirectXCommon;
	MyEngine* engine = new MyEngine;

	//アプリケーションの開始
	winApp->StartApp();
	directXCommon->Initialize(winApp->GetHwnd());
	engine->Initialize();

	Vector4 triangleData[10][3];
	Triangle* triangle[4];
	Vector4 material[10];

	for (int i = 0; i < 4; i++)
	{
		triangleData[i][0] = { -0.2f,-0.8f + (i * 0.5f),0.0f,1.5f };
		triangleData[i][1] = { 0.0,-0.4f + (i * 0.5f),0.0f,1.5f };
		triangleData[i][2] = { 0.2f,-0.8f + (i * 0.5f),0.0f,1.5f };
		material[i] = { 1.0f,0.0f,0.0f,1.0f };
		triangle[i] = new Triangle;
		triangle[i]->Initialize(directXCommon, triangleData[i][0], triangleData[i][1], triangleData[i][2], material[i]);
	}


	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		//Windowsにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			engine->Update();

			for (int i = 0; i < 4; i++){
				//三角形描画
				triangle[i]->Draw();
			}

			engine->UpdateEnd();

		}
	}

	for (int i = 0; i < 4; i++) {
		triangle[i]->Finalize();
	}
	winApp->EndApp();
	engine->Finalize();
	directXCommon->PostDraw();

	return 0;
}