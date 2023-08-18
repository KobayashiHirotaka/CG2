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

	ID3D12Device* GetDevice() { return device_; }

	ID3D12GraphicsCommandList* GetcommandList() { return commandList_; }

	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc_; }

	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() { return rtvDesc_; }

	ID3D12DescriptorHeap* GetSrvDescriptorHeap() { return srvDescriptorHeap_; }

private:
	ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

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

	ID3D12Resource* CreateDepthStencilTextureResource(int32_t width, int32_t height);

private:
	WindowsApp* win_;

	int32_t kClientWidth_;
	int32_t kClientHeight_;

	HRESULT hr_;

	//DXGIファクトリーの生成
	IDXGIFactory7* dxgiFactory_;

	//使用するアダプタ用の変数
	IDXGIAdapter4* useAdapter_;

	//D3D12Deviceの生成
	ID3D12Device* device_;

	//コマンドキュー生成
	ID3D12CommandQueue* commandQueue_;

	//コマンドアロケータの生成
	ID3D12CommandAllocator* commandAllocator_;

	//コマンドリストを生成する
	ID3D12GraphicsCommandList* commandList_;

	//スワップチェーン
	IDXGISwapChain4* swapChain_{};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_;

	//ディスクリプタヒープの生成
	ID3D12DescriptorHeap* rtvDescriptorHeap_;
	ID3D12DescriptorHeap* srvDescriptorHeap_;
	ID3D12DescriptorHeap* dsvDescriptorHeap_;

	//深度
	ID3D12Resource* depthStencilResource_;

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};

	//RTVを２つ作るのでディスクリプタを２つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	ID3D12Resource* swapChainResources_[2];

	//Fence
	ID3D12Fence* fence_;
	UINT64 fenceValue_;
	HANDLE fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);

	D3D12_RESOURCE_BARRIER barrier_{};

	//DXCの初期化
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;
	IDxcIncludeHandler* includeHandler_ = nullptr;

	IDxcBlobUtf8* shaderError_ = nullptr;
	IDxcBlob* shaderBlob_ = nullptr;

	//inputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3];

	//blend
	D3D12_BLEND_DESC blendDesc_{};

	//rasterizer
	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	//PSO
	ID3D12PipelineState* graphicsPipelineState_;

	ID3DBlob* signatureBlob_;
	ID3DBlob* errorBlob_;

	ID3D12RootSignature* rootSignature_;

	IDxcBlob* vertexShaderBlob_;

	IDxcBlob* pixelShaderBlob_;

	//viewport
	D3D12_VIEWPORT viewport_{};

	//scissor
	D3D12_RECT scissorRect_{};
	
#ifdef _DEBUG
	ID3D12Debug1* debugController_ = nullptr;
#endif
};


