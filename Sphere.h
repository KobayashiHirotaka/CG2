#pragma once
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"
#include "VertexData.h"
#include "Material.h"
#include "Light.h"
#include <wrl.h>
#include <numbers>

class Sphere
{
public:

	void Initialize();
	void Draw(const WorldTransform& transform, const ViewProjection& viewProjection, const uint32_t& textureHandle);

private:
	DirectXCommon* dxCommon_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	Light* light_ = nullptr;

	//分割数
	const int kSubdivision_ = 16;

	//どのライトを使うか
	int32_t lightFlag = Lighting::NotDo;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceSphere_ = nullptr;

	VertexData* vertexDataSphere_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceSphere_ = nullptr;

	Material* materialDataSphere_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSphere_ = nullptr;

	uint32_t* indexDataSphere_;

	D3D12_INDEX_BUFFER_VIEW indexBufferViewSphere_{};

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	void CreateVertexBufferViewSphere();
	void CreateIndexBufferViewSphere();

};
