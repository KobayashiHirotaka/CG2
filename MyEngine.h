#pragma once
#include "DirectXCommon.h"
#include "MyMath.h"
#include "Matrix4x4.h"
#include "VertexData.h"
#include "externals/DirectXTex/DirectXTex.h"

class MyEngine
{
public:
	void Initialize(DirectXCommon* dxCommon);

	void Update();

	void Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material, const Matrix4x4& wvpData);

	void Release();

	void ImGui();

	DirectX::ScratchImage LoadTexture(const std::string& filePath);

private:
	void CreateVerteexBufferView();

	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

	DirectX::ScratchImage OpenImage(const std::string& filePath);

	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	void UploadTexturData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

private:
	DirectXCommon* dxCommon_;

	ID3D12Resource* vertexResource_;

	//頂点リソースにデータを書き込む
	VertexData* vertexData_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	ID3D12Resource* materialResource_;

	Vector4* materialData_;

	//WVP用のリソース
	ID3D12Resource* wvpResource_;

	Matrix4x4* wvpData_;

	ID3D12Resource* textureResource_ = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;

	Transform transform_
	{
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f}
	};

	Matrix4x4 worldMatrix_;
};
