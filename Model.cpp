#include "Model.h"

void Model::Initialize()
{
	dxCommon_->GetInstance();
	textureManager_->GetInstance();
	light_->GetInstance();
}

void Model::Draw(const WorldTransform& transform, const ViewProjection& viewProjection, const ModelData& modelData)
{
	vertexResourceObj_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataObj_));
	std::memcpy(vertexDataObj_, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

	materialResourceObj_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataObj_));

	materialDataObj_->enableLighting = false;
	materialDataObj_->color = material;
	materialDataObj_->uvTransform = MakeIdentity4x4();

	//書き込むためのアドレス取得
	transformationMatrixResourceObj_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataObj_));

	Matrix4x4 worldMatrixObj = MakeAffineMatrix(transformObj_.scale, transformObj_.rotate, transformObj_.translate);
	transformationMatrixDataObj_->WVP = Multiply(worldMatrixObj, ViewMatrix);
	transformationMatrixDataObj_->World = MakeIdentity4x4();

	dxCommon_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxCommon_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewObj_);

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResourceObj_->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceObj_->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_[modelData.TextureIndex]);

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}
