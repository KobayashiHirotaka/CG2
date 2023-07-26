#pragma once
#include<Windows.h>
#include<cstdint>
#include <string>
#include<format>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dxgidebug.h>
#include<dxcapi.h>
#include<Vector>

#include"DirectXCommon.h"
#include"WindowsApp.h"
#include"Triangle.h"
#include "ConvertString.h"
#include"Vector4.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

class MyEngine
{
public:
	/// コンストラクタ
	MyEngine();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~MyEngine();

	/// 初期化
	void Initialize();

	/// 更新
	void Update();

	/// 更新処理の終了
	void UpdateEnd();

	/// 三角形描画
	//void Draw(const Vector4& a, const Vector4& b, const Vector4& c);

	/// 解放処理
	void End();

private:
	//コンパイルシェーダー関数
	IDxcBlob* CompileShader
	(//compilerするshaderファイルへのパス
		const std::wstring& filePath,
		//Compilerに使用するprofile
		const wchar_t* profile,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	// DXCの初期化
	void DXCInitialize();

	// RootSignatureの生成
	void CreateRootSignature();

	// InputLayerの設定
	void SettingInputLayout();

	// BlendStateの設定
	void SettingBlendState();

	// RasterizerStateの設定
	void SettingRasterizerState();

	/// シェーダー
	void ShaderCompile();

	/// PSOの生成
	void CreatePSO();

	/// 頂点データ用のリソース
	void VertexResource();

	/// 描画処理
	void Render();

	/// 画面描画の終わり
	void StateChange();

	/// 解放処理
	void Relese();

	DirectXCommon* directXCommon_ = new DirectXCommon;
	WindowsApp winApp_;

	//描画出来る最大個数
	Triangle* triangle_[11];

	//三角形が描画されている数
	int triangleCount_;

	HRESULT hr_;

	IDxcUtils* dxcUtils_ = nullptr;

	IDxcCompiler3* dxcCompiler_ = nullptr;

	IDxcIncludeHandler* includeHandler_ = nullptr;

	//RootSignatureの作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};

	//バイナリを元に生成
	ID3D12RootSignature* rootSignature_ = nullptr;

	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[1] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc_{};

	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	//PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_{};

	//実際に生成
	ID3D12PipelineState* graphicsPipelineState_ = nullptr;

	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc_{};

	//実際に頂点リソースを作る
	ID3D12Resource* vertexResource_;

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//ビューポート
	D3D12_VIEWPORT viewport_{};

	//シザー矩形
	D3D12_RECT scissorRect_{};

	UINT backBufferIndex;

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};

	//シリアライズしてバイナリにする
	ID3DBlob* signatureBlob_ = nullptr;
	ID3DBlob* errorBlob_ = nullptr;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;

	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle_[2];
	uint64_t fenceValue_;

	//頂点リソースにデータを書き込む
	Vector4* vertexData_;
};
