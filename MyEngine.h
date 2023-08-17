#pragma once
#include"DirectXCommon.h"
#include"MyMath.h"
#include"Matrix4x4.h"
#include"VertexData.h"
#include "Sphere.h"
#include"externals/DirectXTex/DirectXTex.h"

class MyEngine
{
public:
	void Initialize(DirectXCommon* dxCommon, int32_t kClientWidth, int32_t kClientHeight);

	void Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material, const Matrix4x4& ViewMatrix);

	void DrawSprite(const Vector4& LeftTop, const Vector4& LeftBottom, const Vector4& RightTop, const Vector4& RightBottom);

	void DrawSphere(const Sphere& sphere, const Matrix4x4& ViewMatrix);

	void ImGui();

	void Release();

	void LoadTexture(const std::string& filePath);

private:
	void CreateVertexBufferView();

	void CreateVertexBufferViewSprite();

	void CreateVertexBufferViewSphere();
	
	void SettingColor();

	void SettingWVP();

	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);
	DirectX::ScratchImage OpenImage(const std::string& filePath);
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

private:
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	Transform transform_
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	Transform transformSprite_
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	Transform transformSphere_
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	HRESULT hr_;
	DirectXCommon* dxCommon_ = nullptr;

	
	ID3D12Resource* vertexResource_ = nullptr;
	
	VertexData* vertexData_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	
	ID3D12Resource* materialResource_ = nullptr;

	Vector4* materialData_ = nullptr;

	ID3D12Resource* wvpResource_ = nullptr;

	Matrix4x4* wvpData_ = nullptr;
	
	ID3D12Resource* textureResource_ = nullptr;

	ID3D12Resource* intermediateResource_ = nullptr;
	
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;

	//Sprite
	ID3D12Resource* vertexResourceSprite_ = nullptr;
	
	VertexData* vertexDataSprite_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_{};
	
	ID3D12Resource* transformationMatrixResourceSprite_ = nullptr;

	Matrix4x4* transformationMatrixDataSprite_ = nullptr;

	//Sphere
	const int kSubdivision_ = 16;

	ID3D12Resource* vertexResourceSphere_ = nullptr;

	VertexData* vertexDataSphere_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere_{};

	ID3D12Resource* transformationMatrixResourceSphere_ = nullptr;

	Matrix4x4* transformationMatrixDataSphere_ = nullptr;
};