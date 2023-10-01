#include "Triangle.h"

void Triangle::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	vertexResource_ = CreateBufferResource(sizeof(VertexData) * 3);
	materialResource_ = CreateBufferResource(sizeof(Material));
	wvpResource_ = CreateBufferResource(sizeof(TransformationMatrix));

	CreateVertexBufferView();
	vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//左下
	vertexData_[0].position = { -0.5f,0.0f,0.0f,1.0f };
	vertexData_[0].texcoord = { 0.0f,1.0f };

	//上
	vertexData_[1].position = { 0.0f,1.0f,0.0f,1.0f };
	vertexData_[1].texcoord = { 0.5f,0.0f };

	//右下
	vertexData_[2].position = { 0.5f,0.0f,0.0f,1.0f };
	vertexData_[2].texcoord = { 1.0f,1.0f };

	//色を書き込むアドレスを取得
	materialResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	//色情報を書き込む
	materialData_->color = color_;
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();
}

void Triangle::Draw(const Matrix4x4& ViewMatrix, const  uint32_t textureHandle)
{
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTranformTriAngle_.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTranformTriAngle_.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTranformTriAngle_.translate));
	materialData_->uvTransform = uvTransformMatrix;

	//行列を作る
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	//WVPを書き込むアドレスを取得
	wvpResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));

	//単位行列を書き込む
	*wvpData_ = Multiply(worldMatrix, ViewMatrix);
	dxCommon_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	dxCommon_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//色用のCBufferの場所を特定
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());

	//WVP用のCBufferの場所を特定
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_.Get()->GetGPUVirtualAddress());

	//SRVのDescriptorTableの先頭を設定　2はrootParameter[2]の2
	dxCommon_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(textureHandle));

	dxCommon_->GetcommandList()->DrawInstanced(3, 1, 0, 0);
}

void Triangle::ImGui(const char* Title)
{
	ImGui::Begin(Title);
	ImGui::SliderFloat3("Scale", &transform_.scale.x, 1, 30, "%.3f");
	ImGui::SliderFloat3("Rotate", &transform_.rotate.x, -7, 7, "%.3f");
	ImGui::SliderFloat3("Translate", &transform_.translate.x, -2, 2, "%.3f");
	ImGui::DragFloat2("UVTranslate", &uvTranformTriAngle_.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTranformTriAngle_.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTranformTriAngle_.rotate.z);
	ImGui::ColorPicker4("Color", &color_.x);
	ImGui::End();
}

void Triangle::CreateVertexBufferView()
{
	vertexBufferView_.BufferLocation = vertexResource_.Get()->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 3;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

Microsoft::WRL::ComPtr<ID3D12Resource> Triangle::CreateBufferResource(size_t sizeInBytes)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC ResourceDesc{};
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInBytes;
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//頂点リソースを作る
	HRESULT hr = dxCommon_->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));
	return Resource;
}


