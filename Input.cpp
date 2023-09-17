#include "Input.h"
Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void Input::Initialize(WindowsApp* win) {
	//DirectInputのオブジェクトを作成
	HRESULT hr = DirectInput8Create(win->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));
	//キーボードデバイスを生成
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(hr));
	//入力データ形式のセット
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));
	//排他制御レベルのセット
	hr = keyboard->SetCooperativeLevel(win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));
	keys = {};
	preKeys = {};
}

void Input::Update()
{
	preKeys = keys;
	//キーボード情報の取得開始
	keyboard->Acquire();
	keys = {};
	//全てのキーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(keys), &keys);

}

bool Input::PushKey(uint8_t keyNumber)const
{
	if (!keys[keyNumber] && preKeys[keyNumber]) {
		return true;
	}
	else {
		return false;
	}
}
bool Input::PressKey(uint8_t keyNumber)const
{
	if (keys[keyNumber]) {
		return true;
	}
	else {
		return false;
	}
}


bool Input::IsReleseKey(uint8_t keyNumber)const
{
	if (keys[keyNumber] && !preKeys[keyNumber]) {
		return true;
	}
	else {
		return false;
	}
}
