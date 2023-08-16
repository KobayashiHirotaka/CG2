#include "MyEngine.h"

void MyEngine::Initialize(DirectXCommon* dxCommon, int32_t kClientWidth, int32_t kClientHeight)
{
	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;
	dxCommon_ = dxCommon;

	CreateVertexBufferView();
	CreateVertexBufferViewSprite();

	SettingColor();
	SettingWVP();
}

void MyEngine::Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material, const Matrix4x4& ViewMatrix)
{
	//左下
	vertexData_[0].position = a;
	vertexData_[0].texcoord = { 0.0f,1.0f };
	//上
	vertexData_[1].position = b;
	vertexData_[1].texcoord = { 0.5f,0.0f };
	//右下
	vertexData_[2].position = c;
	vertexData_[2].texcoord = { 1.0f,1.0f };

	vertexData_[3].position = { -0.7f,-0.5f,0.5f,1.0f };
	vertexData_[3].texcoord = { 0.0f,1.0f };

	vertexData_[4].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData_[4].texcoord = { 0.5f,0.0f };
	
	vertexData_[5].position = { 0.7f,-0.5f,-0.5f,1.0f };
	vertexData_[5].texcoord = { 1.0f,1.0f };
	
	*materialData_ = material;

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	*wvpData_ = Multiply(worldMatrix, ViewMatrix);

	//VBVを設定
	dxCommon_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定する
	dxCommon_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を特定
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
	dxCommon_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
	//描画
	dxCommon_->GetcommandList()->DrawInstanced(6, 1, 0, 0);
}

void MyEngine::DrawSprite(const Vector4& LeftTop, const Vector4& LeftBottom, const Vector4& RightTop, const Vector4& RightBottom)
{
	vertexDataSprite_[0].position = LeftBottom;
	vertexDataSprite_[0].texcoord = { 0.0f,1.0f };
	
	vertexDataSprite_[1].position = LeftTop;
	vertexDataSprite_[1].texcoord = { 0.0f,0.0f };

	vertexDataSprite_[2].position = RightBottom;
	vertexDataSprite_[2].texcoord = { 1.0f,1.0f };

	vertexDataSprite_[3].position = LeftTop;
	vertexDataSprite_[3].texcoord = { 0.0f,0.0f };

	vertexDataSprite_[4].position = RightTop;
	vertexDataSprite_[4].texcoord = { 1.0f,0.0f };

	vertexDataSprite_[5].position = RightBottom;
	vertexDataSprite_[5].texcoord = { 1.0f,1.0f };

	//書き込むためのアドレス取得
	transformationMatrixResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite_));

	Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transformSprite_.scale, transformSprite_.rotate, transformSprite_.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 ProjectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(kClientWidth_), float(kClientHeight_), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, ProjectionMatrixSprite));
	*transformationMatrixDataSprite_ = worldViewProjectionMatrixSprite;

	dxCommon_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を特定
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite_);

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite_->GetGPUVirtualAddress());
	dxCommon_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
	dxCommon_->GetcommandList()->DrawInstanced(6, 1, 0, 0);
}

void MyEngine::ImGui()
{
#pragma region TriAngleImGui
	ImGui::ShowDemoWindow();

	/*ImGui::Begin("TriAngle");
	float ImGuiScale[Vector3D] = { transform.scale.x,transform.scale.y ,transform.scale.z };
	ImGui::SliderFloat3("Scale", ImGuiScale, 1, 30, "%.3f");
	transform.scale = { ImGuiScale[x],ImGuiScale[y],ImGuiScale[z] };
	float ImGuiRotate[Vector3D] = { transform.rotate.x,transform.rotate.y ,transform.rotate.z };
	ImGui::SliderFloat3("Rotate", ImGuiRotate, 0, 7, "%.3f");
	transform.rotate = { ImGuiRotate[x],ImGuiRotate[y],ImGuiRotate[z] };
	float ImGuiTranslate[Vector3D] = { transform.translate.x,transform.translate.y ,transform.translate.z };
	ImGui::SliderFloat3("Translate", ImGuiTranslate, -2, 2, "%.3f");
	transform.translate = { ImGuiTranslate[x],ImGuiTranslate[y],ImGuiTranslate[z] };
	ImGui::End();*/
#pragma endregion
#pragma region SpriteImGui

	/*ImGui::Begin("Sprite");
	float ImGuiScaleSprite[Vector3D] = { transformSprite.scale.x,transformSprite.scale.y ,transformSprite.scale.z };
	ImGui::SliderFloat3("ScaleSprite", ImGuiScaleSprite, 1, 30, "%.3f");
	transformSprite.scale = { ImGuiScaleSprite[x],ImGuiScaleSprite[y],ImGuiScaleSprite[z] };
	float ImGuiRotateSprite[Vector3D] = { transformSprite.rotate.x,transformSprite.rotate.y ,transformSprite.rotate.z };
	ImGui::SliderFloat3("RotateSprite", ImGuiRotateSprite, 0, 7, "%.3f");
	transformSprite.rotate = { ImGuiRotateSprite[x],ImGuiRotateSprite[y],ImGuiRotateSprite[z] };
	float ImGuiTranslateSprite[Vector3D] = { transformSprite.translate.x,transformSprite.translate.y ,transformSprite.translate.z };
	ImGui::SliderFloat3("TranslateSprite", ImGuiTranslateSprite, -640, 640, "%.3f");
	transformSprite.translate = { ImGuiTranslateSprite[x],ImGuiTranslateSprite[y],ImGuiTranslateSprite[z] };
	ImGui::End();*/
#pragma endregion
}

void MyEngine::Release()
{
	vertexResource_->Release();
	materialResource_->Release();
	wvpResource_->Release();
	textureResource_->Release();
	intermediateResource_->Release();
	vertexResourceSprite_->Release();
	transformationMatrixResourceSprite_->Release();
}
void MyEngine::CreateVertexBufferView()
{
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * 6);
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void MyEngine::SettingColor()
{
	materialResource_ = CreateBufferResource(sizeof(Vector4) * 3);
	//書き込むアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void MyEngine::SettingWVP()
{
	wvpResource_ = CreateBufferResource(sizeof(Matrix4x4));
	//WVPを書き込むアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
}

void MyEngine::LoadTexture(const std::string& filePath)
{
	//Textureを読んで転送する
	DirectX::ScratchImage mipImages = OpenImage(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textureResource_ = CreateTextureResource(dxCommon_->GetDevice(), metadata);
	intermediateResource_ = UploadTextureData(textureResource_, mipImages);

	//metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	//SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU_ = dxCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU_ = dxCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	//先頭はImGuiが使っているので次のを使う
	textureSrvHandleCPU_.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU_.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの作成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource_, &srvDesc, textureSrvHandleCPU_);
}

DirectX::ScratchImage MyEngine::OpenImage(const std::string& filePath)
{
	//テクスチャファイルを読み込みプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	hr_ = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr_));
	//ミップマップの作成
	DirectX::ScratchImage mipImage{};
	hr_ = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImage);

	return mipImage;
}

ID3D12Resource* MyEngine::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
{
	//metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);
	//利用するheapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	//Resourceの生成
	ID3D12Resource* resource = nullptr;
	hr_ = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&resource)
	);

	assert(SUCCEEDED(hr_));

	return resource;
}

ID3D12Resource* MyEngine::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	std::vector<D3D12_SUBRESOURCE_DATA>subresources;
	DirectX::PrepareUpload(dxCommon_->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	ID3D12Resource* intermediateResource = CreateBufferResource(intermediateSize);
	UpdateSubresources(dxCommon_->GetcommandList(), texture, intermediateResource, 0, 0, UINT(subresources.size()), subresources.data());

	//Tetureへの転送後は利用できるようにD3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	dxCommon_->GetcommandList()->ResourceBarrier(1, &barrier);

	return intermediateResource;
}

void MyEngine::CreateVertexBufferViewSprite()
{
	vertexResourceSprite_ = CreateBufferResource(sizeof(VertexData) * 6);
	transformationMatrixResourceSprite_ = CreateBufferResource(sizeof(Matrix4x4));
	vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));
	
	vertexBufferViewSprite_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();

	vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 6;

	vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);
}

ID3D12Resource* MyEngine::CreateBufferResource(size_t sizeInBytes)
{
	ID3D12Resource* Resource = nullptr;
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
	
	hr_ = dxCommon_->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr_));

	return Resource;
}

