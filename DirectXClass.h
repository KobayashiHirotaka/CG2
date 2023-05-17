#pragma once
#include <Windows.h>
#include "WindowsClass.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <string>
#include <format>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")


class DirectXClass
{
public:



	static DirectXClass* GetInstance();


private:

	WindowsClass* win_;

	// DXGIデバイス初期化
	void InitializeDXGIDevice();

	void InitializeCommand();

	void CreateSwapChain();

	void CreateFinalRenderTargets();

	void PreDraw();

	//DXGIファクトリーの生成
	IDXGIFactory7* dxgiFactory = nullptr;

	//使用するアダプタ用の変数。最初にnullptrを入れておく
	IDXGIAdapter4* useAdapter = nullptr;

	ID3D12Device* device = nullptr;

	// コマンドキューを生成する
	ID3D12CommandQueue* commandQueue = nullptr;

	// ディスクリプタヒープの生成
	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;

};
