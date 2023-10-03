#pragma once
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyImGui.h"
#include "Light.h"
#include "ModelData.h"

class Model
{
public:
	void Initialize();

	void Draw(const WorldTransform& transform, const ViewProjection& viewProjection, const uint32_t& textureHandle);


private:
	DirectXCommon* dxCommon_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	Light* light_ = nullptr;

	int32_t lightFlag = Lighting::NotDo;
	
	ModelData modelData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceObj_;

	VertexData* vertexDataObj_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewObj_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceObj_ = nullptr;

	Material* materialDataObj_ = nullptr;

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

};

