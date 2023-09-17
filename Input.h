#pragma once
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include <array>
#include <wrl.h>
#include "WindowsApp.h"

class Input
{
public:
	static Input* GetInstance();
	void Initialize(WindowsApp* win);
	void Update();

	/// 押した時
	bool PushKey(uint8_t keyNumber)const;
	/// 押している間
	bool PressKey(uint8_t keyNumber)const;
	/// 離した時
	bool IsReleseKey(uint8_t keyNumber)const;

private:
	/*Input() = default;
	~Input() = default;*/
	static Input* input_;
	Microsoft::WRL::ComPtr<IDirectInput8>directInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard = nullptr;
	std::array<BYTE, 256> keys;
	std::array<BYTE, 256> preKeys;
};

