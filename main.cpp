#include <Windows.h>
#include "ConvertString.h"
#include <cstdint>
#include <format>
#include <d3d12.h>
#include <cassert>
#include "Vector4.h"
#include "MyEngine.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	MyEngine* engine = new MyEngine();

	engine->Initialize();

#pragma region メインループ

	MSG msg{};
	// ウィンドウの×ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		// Windowにメッセージが来ていたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// ループ処理
			engine->BeginFrame();

			engine->Draw();

			engine->EndFrame();
		}
	}
	// 解放処理
	engine->Release();
#pragma endregion

	return 0;
}