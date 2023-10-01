#pragma once
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "MyMath.h"
#include "VertexData.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include <wrl.h>

class Triangle
{
public:
	void Initialize();
	void Draw(const Matrix4x4& ViewMatrix, const uint32_t textureHandle);
	void ImGui(const char* Title);

private:
	static const int kMaxTexture_ = 3;
	bool CheckTextureIndex_[kMaxTexture_];

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	DirectXCommon* dxCommon_ = nullptr;

	TextureManager* textureManager_ = nullptr;

	Transform transform_
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	Transform uvTranformTriAngle_
	{
	    {1.0f,1.0f,1.0f},
	    {0.0f,0.0f,0.0f},
	    {0.0f,0.0f,0.0f}
	};

	//バーテックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource>vertexResource_ = nullptr;

	//頂点データ
	VertexData* vertexData_ = nullptr;

	//バーテックスバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	//色データ
	Material* materialData_ = nullptr;

	//WVPリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_ = nullptr;

	//WVPデータ
	Matrix4x4* wvpData_ = nullptr;

	void CreateVertexBufferView();
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
};

