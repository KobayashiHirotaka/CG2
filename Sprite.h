#pragma once
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "MyMath.h"
#include "VertexData.h"
#include "Material.h"
#include <wrl.h>

class Sprite
{
public:
	void Initialize(const Vector4& LeftTop, const Vector4& LeftBottom, const Vector4& RightTop, const Vector4& RightBottom);
	void DrawSprite(const uint32_t TextureHandle);
	void ImGui(const char* Title);

private:
	DirectXCommon* directX_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceSprite_ = nullptr;

	VertexData* vertexDataSprite_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceSprite_ = nullptr;

	Material* materialDataSprite_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSprite_ = nullptr;

	uint32_t* indexDataSprite_;

	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite_{};

	Transform uvTransformSprite_
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	void CreateVertexBufferViewSprite();
	void CreateIndexBufferViewSprite();
};

