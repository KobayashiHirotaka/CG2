#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include"ConvertString.h"
#include<format>
#include<dxgidebug.h>
#include"WindowsApp.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")


class DirectXCommon
{
public:
	// コンストラクタ
	DirectXCommon();

	// デストラクタ
	~DirectXCommon();

	// メインループ前の初期化
	void Initialize(HWND hwnd);

	// 描画前の処理
	void PreDraw();

	// 描画後の処理
	void PostDraw();

	// getter
	ID3D12Device* GetDevice() const { return device_; }
	ID3D12GraphicsCommandList* GetCommandList()const { return  commandList_; };
	//スワップチェーン
	IDXGISwapChain4* GetSwapChain()const { return swapChain_; };
	ID3D12Resource* GetSwapChainResource(int index)const { return swapChainResources_[index]; };
	D3D12_CPU_DESCRIPTOR_HANDLE& GetRTVHandle(int index)const
	{
		assert(index >= 0 && index < 2);
		return rtvHandles_[index];
	}
	//コマンドキュー
	ID3D12CommandQueue* GetCommandQueue()const { return commandQueue_; };
	//コマンドアロケータ
	ID3D12CommandAllocator* GetCommandAllocator()const { return commandAllocator_; };
	//初期値0でFenceを作る
	ID3D12Fence* GetFence()const { return fence_; };
	//フェンス値
	uint64_t GetFenceValue()const { return fenceValue_; };
	HANDLE GetFenceEvent() { return fenceEvent_; };

	// setter

private:
	// ウィンドウ関連の管理
	WindowsApp winApp_;
	HWND hwnd_;

	// DXGIファクトリーの生成
	IDXGIFactory7* dxgiFactory = nullptr;

	// 使用するアダプタ用の変数。
	IDXGIAdapter4* useAdapter = nullptr;

	// ディスクリプタヒープ
	ID3D12DescriptorHeap* rtvdescriptorHeap = nullptr;

	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	HRESULT hr_;

	// デバイスの生成
	static ID3D12Device* device_;

	// コマンドリスト
	static ID3D12GraphicsCommandList* commandList_;

	// スワップチェーン
	static IDXGISwapChain4* swapChain_;

	// SwapChainからResourceを引っ張ってくる
	static ID3D12Resource* swapChainResources_[2];

	// RTVを二つ作るのでディスクリプタを二つ用意
	static D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];

	// コマンドキュー
	static ID3D12CommandQueue* commandQueue_;

	// コマンドアロケータ
	static ID3D12CommandAllocator* commandAllocator_;

	// 初期値0でFenceを作る
	static ID3D12Fence* fence_;

	// フェンス値
	uint64_t fenceValue_;

	// フェンスイベント
	HANDLE fenceEvent_;

	//DXGIファクトリーの生成
	void InitializeDXGIDevice();
	// コマンド関連初期化
	void InitializeCommand();

	// スワップチェーンの生成
	void CreateSwapChain();

	// レンダーターゲット生成
	void CreateFinalRenderTargets();

	//スワップチェーンからもってくる
	void PullResourceSwapChain();

	//RTV作成
	void CreateRTV();

	// フェンス生成
	void CreateFence();

	//コマンドのキック
	void CommandKick();

	//解放
	void Relese();
};


