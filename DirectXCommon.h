#pragma once
#include "WindowsApp.h"
#include "ConvertString.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include "dxgidebug.h"
#include "Vector3.h"
#include "Vector4.h"
#include <cassert>
#include "externals/DirectXTex/d3dx12.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"dxguid.lib")

class DirectXCommon
{
public:
	void Initialize(WindowsApp* win, int32_t kClientWidth, int32_t kClientHeight);

	void PreDraw();

	void PostDraw();

	void Release();

	ID3D12Device* GetDevice() { return device; }

	ID3D12GraphicsCommandList* GetcommandList() { return commandList; }

	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc; }

	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() { return rtvDesc; }

	ID3D12DescriptorHeap* GetSrvDescriptorHeap() { return srvDescriptorHeap; }

private:
	ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	ID3D12Resource* CreateDepthStencilTextureResource(int32_t width, int32_t height);

	void CreateDXGIDevice();

	void CreateCommand();

	void CreateSwapChain();

	void CreateFinalRenderTargets();

	void CreateFence();

	void CreateRootSignature();
	void CreateInputLayOut();
	void CreateBlendState();
	void CreateRasterizerState();
	void CreatePSO();

	void CreateViewport();
	void CreateScissorRect();

	void CreateDxcCompiler();
	IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler);

private:
	WindowsApp* win_;
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	HRESULT hr;
	//TransitionBarrier
	D3D12_RESOURCE_BARRIER barrier{};
	//DXGIファクトリー
	IDXGIFactory7* dxgiFactory = nullptr;
	//アダプターを作成
	IDXGIAdapter4* useAdapter = nullptr;
	//デバイス
	ID3D12Device* device = nullptr;
	//コマンドキューを生成
	ID3D12CommandQueue* commandQueue = nullptr;
	//コマンドアロケータを作成
	ID3D12CommandAllocator* commandAllocator = nullptr;
	//コマンドリストを作成
	ID3D12GraphicsCommandList* commandList = nullptr;
	//スワップチェーンを作成
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	IDXGISwapChain4* swapChain = nullptr;
	//RTVDescriptorHeap
	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
	//SRVDescriptorHeap
	ID3D12DescriptorHeap* srvDescriptorHeap = nullptr;
	//DSVDescriptorHeap
	ID3D12DescriptorHeap* dsvDescriptorHeap = nullptr;
	//深度
	ID3D12Resource* depthStencilResource = nullptr;
	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//SwapChainからResourceを持ってくる
	ID3D12Resource* swapChainResources[2] = { nullptr };
	//RTVを2つつくるので2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	//Fenceを作る
	ID3D12Fence* fence = nullptr;
	uint64_t fenceValue = 0;
	//FenceのSignalを待つイベント
	HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//dxCompiler初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;
	//コンパイルシェーダー
	IDxcBlobUtf8* shaderError = nullptr;
	IDxcBlob* shaderBlob = nullptr;
	//ルートシグネチャ―
	ID3D12RootSignature* rootSignature = nullptr;
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	//インプットレイアウト
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	//ブレンドステート
	D3D12_BLEND_DESC blendDesc{};
	//ラスタライザステート
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//シェーダーコンパイル
	IDxcBlob* vertexShaderBlob = nullptr;
	IDxcBlob* pixelShaderBlob = nullptr;
	//PSO
	ID3D12PipelineState* graphicsPipelineState = nullptr;
	//ビューポート
	D3D12_VIEWPORT viewport{};
	//シザー
	D3D12_RECT scissorRect{};
#ifdef _DEBUG
	ID3D12Debug1* debugController = nullptr;
#endif
};


