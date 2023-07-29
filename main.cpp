#include "MyEngine.h"
#include "Triangle.h"

const wchar_t kWindowTitle[] = { L"CG2" };

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//初期化
	WindowsApp* win = nullptr;
	MyEngine* engine = new MyEngine;
	engine->Initialization(win, kWindowTitle, 1280, 720);

	engine->Initialize();

	Vector4 triangleData[3][3];
	Vector4 material[3];

	//左
	triangleData[0][0] = {-0.8f,-0.2f,0.0f,1.0f};
	triangleData[0][1] = {-0.6f,0.2f,0.0f,1.0f};
	triangleData[0][2] = {-0.4f,-0.2f,0.0f,1.0f};
	material[0] = { 1.0f,1.0f,0.0f,1.0f };

	//真ん中
	triangleData[1][0] = {-0.2f,-0.2f,0.0f,1.0f};
	triangleData[1][1] = {0.0f,0.2f,0.0f,1.0f};
	triangleData[1][2] = {0.2f,-0.2f,0.0f,1.0f};
	material[1] = { 0.0f,1.0f,1.0f,1.0f };

	//右
	triangleData[2][0] = {0.4f,-0.2f,0.0f,1.0f};
	triangleData[2][1] = {0.6f,0.2f,0.0f,1.0f};
	triangleData[2][2] = {0.8f,-0.2f,0.0f,1.0f};
	material[2] = { 1.0f,0.0f,1.0f,1.0f };


	MSG msg{};

	//ウィンドウのxが押されるまでループ
	while (msg.message != WM_QUIT)
	{
		//windowのメッセージを最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲームの処理
			engine->Update();
			engine->BeginFrame();

			for (int i = 0; i < 3; i++) 
			{
				//三角形描画
				engine->DrawTriangle(triangleData[i][0], triangleData[i][1], triangleData[i][2],material[i]);
			}


			engine->EndFrame();
		}
	}

	engine->Finalize();

	return 0;
}