#include "MyEngine.h"
#include "Triangle.h"

const wchar_t kWindowTitle[] = { L"CG2" };

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//初期化
	WindowsApp* win_ = nullptr;
	MyEngine* engine = new MyEngine;
	engine->Initialization(win_, kWindowTitle, 1280, 720);

	engine->Initialize();

	Vector4 data[15];

	data[0] = { 0.4f,0.4f,0.0f,1.0f };
	data[1] = { 0.6f,0.8f,0.0f,1.0f };
    data[2] = { 0.8f,0.4f,0.0f,1.0f };
   	
    data[3] = { -0.8f,-0.8f,0.0f,1.0f };
    data[4] = { -0.6f,-0.4f,0.0f,1.0f };
    data[5] = { -0.4f,-0.8f,0.0f,1.0f };
   	
    data[6] = { 0.4f,-0.8f,0.0f,1.0f };
    data[7] = { 0.6f,-0.4f,0.0f,1.0f };
    data[8] = { 0.8f,-0.8f,0.0f,1.0f };
   
	data[9] = { -0.8f,0.4f,0.0f,1.0f };
    data[10] = { -0.6f,0.8f,0.0f,1.0f };
    data[11] = { -0.4f,0.4f,0.0f,1.0f };
   	  
    data[12] = { -0.2f,-0.2f,0.0f,1.0f };
    data[13] = { 0.0f,0.2f,0.0f,1.0f };
    data[14] = { 0.2f,-0.2f,0.0f,1.0f };


	MSG msg{};

	//ウィンドウのxが押されるまでループ
	while (msg.message != WM_QUIT) {
		//windowのメッセージを最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//ゲームの処理
			engine->Update();
			engine->BeginFrame();


			//三角形描画
			engine->DrawTriangle(data[0], data[1], data[2]);
			
			engine->DrawTriangle(data[3], data[4], data[5]);
			
			engine->DrawTriangle(data[6], data[7], data[8]);
			
			engine->DrawTriangle(data[9], data[10], data[11]);
			
			engine->DrawTriangle(data[12], data[13], data[14]);

			
			engine->EndFrame();
		}
	}

	engine->Finalize();

	return 0;
}