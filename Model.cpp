#include "Model.h"

void Model::Initialize()
{
	dxCommon_->GetInstance();
	textureManager_->GetInstance();
	light_->GetInstance();
}

void Model::Draw(const WorldTransform& transform, const ViewProjection& viewProjection, const uint32_t& textureHandle)
{
	vertexResourceObj_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataObj_));
	std::memcpy(vertexDataObj_, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

	materialResourceObj_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataObj_));

	materialDataObj_->enableLighting = lightFlag;
	materialDataObj_->color = color_;
	materialDataObj_->uvTransform = MakeIdentity4x4();

	dxCommon_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxCommon_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewObj_);

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(1,transform.constBuff->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(4,viewProjection.constBuff->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResourceObj_->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(textureHandle));

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(3, light_->GetDirectionalLight()->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}
