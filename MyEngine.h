#pragma once
#include"DirectXCommon.h"
#include"MyMath.h"
#include"Matrix4x4.h"
#include"VertexData.h"
#include "Sphere.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "DirectionalLight.h"
#include "externals/DirectXTex/DirectXTex.h"

class MyEngine
{
public:
	void Initialize(DirectXCommon* dxCommon, int32_t kClientWidth, int32_t kClientHeight);

	void Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material, const Matrix4x4& ViewMatrix, const int index);

	void DrawSprite(const Vector4& LeftTop, const Vector4& LeftBottom, const Vector4& RightTop, const Vector4& RightBottom, const Vector4& material, const int index);

	void DrawSphere(const Sphere& sphere, const Matrix4x4& ViewMatrix, const Vector4& material, const int index);

	void ImGui();

	void Release();

	int LoadTexture(const std::string& filePath);

	void VertexReset();

private:
	void CreateVertexBufferView();

	void CreateVertexBufferViewSprite();
	void CreateIndexBufferViewSprite();

	void CreateVertexBufferViewSphere();
	void CreateIndexBufferViewSphere();
	
	void SettingColor();

	void SettingWVP();

	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);
	DirectX::ScratchImage OpenImage(const std::string& filePath);
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

private:
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	static const int kMaxTexture = 2;

	bool CheckTextureIndex[kMaxTexture];

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

	//Triangle
	static const int kMaxTriangle_ = 3;
	static const int kMaxVertex_ = kMaxTriangle_ * 3;
	bool CheckTriangleIndex_[kMaxTriangle_];

	ID3D12Resource* vertexResource_ = nullptr;
	
	VertexData* vertexData_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	
	ID3D12Resource* materialResource_ = nullptr;

	Material* materialData_ = nullptr;

	ID3D12Resource* wvpResource_ = nullptr;

	Matrix4x4* wvpData_ = nullptr;
	
	ID3D12Resource* textureResource_[kMaxTexture];

	ID3D12Resource* intermediateResource_[kMaxTexture];
	
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[kMaxTexture];
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[kMaxTexture];

	//Sprite
	static const int kMaxSprite_ = 3;
	static const int kMaxSpriteVertex_ = kMaxSprite_ * 6;
	bool CheckSpriteIndex_[kMaxSprite_];

	ID3D12Resource* vertexResourceSprite_ = nullptr;
	
	VertexData* vertexDataSprite_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_{};

	ID3D12Resource* materialResourceSprite_ = nullptr;

	Material* materialDataSprite_ = nullptr;
	
	ID3D12Resource* transformationMatrixResourceSprite_ = nullptr;

	TransformationMatrix* transformationMatrixDataSprite_ = nullptr;

	ID3D12Resource* indexResourceSprite_ = nullptr;

	uint32_t* indexDataSprite_;

	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite_{};

	//Sphere
	const int kSubdivision_ = 16;

	ID3D12Resource* vertexResourceSphere_ = nullptr;

	VertexData* vertexDataSphere_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere_{};

	ID3D12Resource* materialResourceSphere_ = nullptr;

	Material* materialDataSphere_ = nullptr;

	ID3D12Resource* transformationMatrixResourceSphere_ = nullptr;

	TransformationMatrix* transformationMatrixDataSphere_ = nullptr;

	ID3D12Resource* indexResourceSphere_ = nullptr;

	uint32_t* indexDataSphere_;

	D3D12_INDEX_BUFFER_VIEW indexBufferViewSphere_{};

	//DescriptorSizeを取得しておく
	uint32_t descriptorSizeSRV_;
	uint32_t descriptorSizeRTV_;
	uint32_t descriptorSizeDSV_;

	ID3D12Resource* directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
};