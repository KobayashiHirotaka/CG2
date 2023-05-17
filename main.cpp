#include <Windows.h>
#include "WindowsClass.h"
#include "DirectXClass.h"
#include <string>
#include <format>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    WindowsClass* win = nullptr;
    DirectXClass* DX = nullptr;

    // ゲームウィンドウの作成
    win = WindowsClass::GetInstance();
    win->GetWindow();

    DX = DirectXClass::GetInstance();

    MSG msg{};

    //ウィンドウの×ボタンが押されるまでループ
    while (msg.message != WM_QUIT) {
        //Windowにメッセージが来てたら最優先で処理させる
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //ゲームの処理

        }
    }
}
