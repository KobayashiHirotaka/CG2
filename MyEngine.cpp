#include "MyEngine.h"

void MyEngine::Initialize(DirectXCommon* dxCommon, int32_t kClientWidth, int32_t kClientHeight)
{
	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;
	dxCommon_ = dxCommon;

	CreateVertexBufferView();
	CreateVertexBufferViewSprite();
	CreateVertexBufferViewSphere();

	SettingColor();
	SettingWVP();

	directionalLightResource_ = CreateBufferResource(sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;

	descriptorSizeSRV_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descriptorSizeRTV_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSizeDSV_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	for (int i = 0; i < kMaxTexture; i++)
	{
		CheckTextureIndex[i] = false;
		textureResource_[i] = nullptr;
		intermediateResource_[i] = nullptr;
	}

}

void MyEngine::Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material, const Matrix4x4& ViewMatrix, const int index)
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
	dxCommon_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_[index]);
	//描画
	dxCommon_->GetcommandList()->DrawInstanced(6, 1, 0, 0);
}

void MyEngine::DrawSprite(const Vector4& LeftTop, const Vector4& LeftBottom, const Vector4& RightTop, const Vector4& RightBottom, const Vector4& material, const int index)
{
	vertexDataSprite_[0].position = LeftBottom;
	vertexDataSprite_[0].texcoord = { 0.0f,1.0f };
	vertexDataSphere_[0].normal = { 0.0f,0.0f,-1.0f };
	
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

	materialResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite_));
	materialDataSprite_->color = material;

	materialDataSprite_->enableLighting = false;

	//書き込むためのアドレス取得
	transformationMatrixResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite_));

	Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transformSprite_.scale, transformSprite_.rotate, transformSprite_.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 ProjectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(kClientWidth_), float(kClientHeight_), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, ProjectionMatrixSprite));
	transformationMatrixDataSprite_->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixDataSprite_->World = MakeIdentity4x4();

	dxCommon_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxCommon_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite_);

	//マテリアルCBufferの場所を特定
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite_->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite_->GetGPUVirtualAddress());
	dxCommon_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_[index]);
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	dxCommon_->GetcommandList()->DrawInstanced(6, 1, 0, 0);
}

void MyEngine::DrawSphere(const Sphere& sphere, const Matrix4x4& ViewMatrix, const Vector4& material, const int index)
{
	//経度分割1つ分の角度φ
	const float kLonEvery = float(std::numbers::pi) * 2.0f / float(kSubdivision_);
	//緯度分割1つ分の角度θ
	const float kLatEvery = float(std::numbers::pi) / float(kSubdivision_);

	//緯度の方向に分割
	for (int latIndex = 0; latIndex < kSubdivision_; ++latIndex)
	{
		float lat = -float(std::numbers::pi) / 2.0f + kLatEvery * latIndex;
		float uvLength = 1.0f / kSubdivision_;

		//経度の方向に分割しながら線を描く
		for (int lonIndex = 0; lonIndex < kSubdivision_; ++lonIndex)
		{
			uint32_t start = (latIndex * kSubdivision_ + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;
			float u = float(lonIndex) / float(kSubdivision_);
			float v = 1.0f - float(latIndex) / float(kSubdivision_);

			vertexDataSphere_[start].position.x = cos(lat) * cos(lon) + sphere.center.x;
			vertexDataSphere_[start].position.y = sin(lat) + sphere.center.y;
			vertexDataSphere_[start].position.w = cos(lat) * sin(lon) + sphere.center.z;
			vertexDataSphere_[start].position.h = 1.0f;
			vertexDataSphere_[start].texcoord = { u,v + uvLength };
			vertexDataSphere_[start].normal.x = vertexDataSphere_[start].position.x;
			vertexDataSphere_[start].normal.y = vertexDataSphere_[start].position.y;
			vertexDataSphere_[start].normal.z = vertexDataSphere_[start].position.w;

			vertexDataSphere_[start + 1].position.x = cos(lat + kLatEvery) * cos(lon) + sphere.center.x;
			vertexDataSphere_[start + 1].position.y = sin(lat + kLatEvery) + sphere.center.y;
			vertexDataSphere_[start + 1].position.w = cos(lat + kLatEvery) * sin(lon) + sphere.center.z;
			vertexDataSphere_[start + 1].position.h = 1.0f;
			vertexDataSphere_[start + 1].texcoord = { u,v };
			vertexDataSphere_[start + 1].normal.x = vertexDataSphere_[start + 1].position.x;
			vertexDataSphere_[start + 1].normal.y = vertexDataSphere_[start + 1].position.y;
			vertexDataSphere_[start + 1].normal.z = vertexDataSphere_[start + 1].position.w;

			vertexDataSphere_[start + 2].position.x = cos(lat) * cos(lon + kLonEvery) + sphere.center.x;
			vertexDataSphere_[start + 2].position.y = sin(lat) + sphere.center.y;
			vertexDataSphere_[start + 2].position.w = cos(lat) * sin(lon + kLonEvery) + sphere.center.z;
			vertexDataSphere_[start + 2].position.h = 1.0f;
			vertexDataSphere_[start + 2].texcoord = { u + uvLength, v + uvLength };
			vertexDataSphere_[start + 2].normal.x = vertexDataSphere_[start + 2].position.x;
			vertexDataSphere_[start + 2].normal.y = vertexDataSphere_[start + 2].position.y;
			vertexDataSphere_[start + 2].normal.z = vertexDataSphere_[start + 2].position.w;

			vertexDataSphere_[start + 3].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery) + sphere.center.x;
			vertexDataSphere_[start + 3].position.y = sin(lat + kLatEvery) + sphere.center.y;
			vertexDataSphere_[start + 3].position.w = cos(lat + kLatEvery) * sin(lon + kLonEvery) + sphere.center.z;
			vertexDataSphere_[start + 3].position.h = 1.0f;
			vertexDataSphere_[start + 3].texcoord = { u + uvLength,v };
			vertexDataSphere_[start + 3].normal.x = vertexDataSphere_[start + 3].position.x;
			vertexDataSphere_[start + 3].normal.y = vertexDataSphere_[start + 3].position.y;
			vertexDataSphere_[start + 3].normal.z = vertexDataSphere_[start + 3].position.w;

			vertexDataSphere_[start + 4].position.x = cos(lat) * cos(lon + kLonEvery) + sphere.center.x;
			vertexDataSphere_[start + 4].position.y = sin(lat) + sphere.center.y;
			vertexDataSphere_[start + 4].position.w = cos(lat) * sin(lon + kLonEvery) + sphere.center.z;
			vertexDataSphere_[start + 4].position.h = 1.0f;
			vertexDataSphere_[start + 4].texcoord = { u + uvLength, v + uvLength };
			vertexDataSphere_[start + 4].normal.x = vertexDataSphere_[start + 4].position.x;
			vertexDataSphere_[start + 4].normal.y = vertexDataSphere_[start + 4].position.y;
			vertexDataSphere_[start + 4].normal.z = vertexDataSphere_[start + 4].position.w;

			vertexDataSphere_[start + 5].position.x = cos(lat + kLatEvery) * cos(lon) + sphere.center.x;
			vertexDataSphere_[start + 5].position.y = sin(lat + kLatEvery) + sphere.center.y;
			vertexDataSphere_[start + 5].position.w = cos(lat + kLatEvery) * sin(lon) + sphere.center.z;
			vertexDataSphere_[start + 5].position.h = 1.0f;
			vertexDataSphere_[start + 5].texcoord = { u,v };
			vertexDataSphere_[start + 5].normal.x = vertexDataSphere_[start + 5].position.x;
			vertexDataSphere_[start + 5].normal.y = vertexDataSphere_[start + 5].position.y;
			vertexDataSphere_[start + 5].normal.z = vertexDataSphere_[start + 5].position.w;
		}
	}

	materialResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSphere_));

	materialDataSphere_->enableLighting = true;
	materialDataSphere_->color = material;

	//書き込むためのアドレス取得
	transformationMatrixResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSphere_));

	Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transformSphere_.scale, transformSphere_.rotate, transformSphere_.translate);
	transformationMatrixDataSphere_->WVP = Multiply(worldMatrixSphere, ViewMatrix);
	transformationMatrixDataSphere_->World = MakeIdentity4x4();

	dxCommon_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を特定
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSphere_->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSphere_);

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSphere_->GetGPUVirtualAddress());
	dxCommon_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_[index]);
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	dxCommon_->GetcommandList()->DrawInstanced(kSubdivision_ * kSubdivision_ * 6, 1, 0, 0);
}

void MyEngine::ImGui()
{
	ImGui::ShowDemoWindow();

	ImGui::Begin("TriAngle,Sprite,Sphere");

	float triangleScale[3] = { transform_.scale.x,transform_.scale.y ,transform_.scale.z };
	ImGui::SliderFloat3("triangleScale", triangleScale, 1, 30, "%.3f");
	transform_.scale = { triangleScale[0],triangleScale[1],triangleScale[2] };

	float triangleRotate[3] = { transform_.rotate.x,transform_.rotate.y ,transform_.rotate.z };
	ImGui::SliderFloat3("triangleRotate", triangleRotate, 0, 7, "%.3f");
	transform_.rotate = { triangleRotate[0],triangleRotate[1],triangleRotate[2] };

	float triangleTranslate[3] = { transform_.translate.x,transform_.translate.y ,transform_.translate.z };
	ImGui::SliderFloat3("triangleTranslate", triangleTranslate, -2, 2, "%.3f");
	transform_.translate = { triangleTranslate[0],triangleTranslate[1],triangleTranslate[2] };

	float spriteScale[3] = { transformSprite_.scale.x,transformSprite_.scale.y ,transformSprite_.scale.z };
	ImGui::SliderFloat3("spriteScale", spriteScale, 1, 30, "%.3f");
	transformSprite_.scale = { spriteScale[0],spriteScale[1],spriteScale[2] };

	float spriteRotate[3] = { transformSprite_.rotate.x,transformSprite_.rotate.y ,transformSprite_.rotate.z };
	ImGui::SliderFloat3("spriteRotate", spriteRotate, 0, 7, "%.3f");
	transformSprite_.rotate = { spriteRotate[0],spriteRotate[1],spriteRotate[2] };

	float spriteTranslate[3] = { transformSprite_.translate.x,transformSprite_.translate.y ,transformSprite_.translate.z };
	ImGui::SliderFloat3("spriteTranslate", spriteTranslate, -640, 640, "%.3f");
	transformSprite_.translate = { spriteTranslate[0],spriteTranslate[1],spriteTranslate[2] };

	float sphereScale[3] = { transformSphere_.scale.x,transformSphere_.scale.y ,transformSphere_.scale.z };
	ImGui::SliderFloat3("sphereScale", sphereScale, -3, 3, "%.3f");
	transformSphere_.scale = { sphereScale[0],sphereScale[1],sphereScale[2] };

	float sphereRotate[3] = { transformSphere_.rotate.x,transformSphere_.rotate.y ,transformSphere_.rotate.z };
	ImGui::SliderFloat3("sphereRotate", sphereRotate, -7, 7, "%.3f");
	transformSphere_.rotate = { sphereRotate[0],sphereRotate[1],sphereRotate[2] };

	float sphereTranslate[3] = { transformSphere_.translate.x,transformSphere_.translate.y ,transformSphere_.translate.z };
	ImGui::SliderFloat3("sphereTranslate", sphereTranslate, -3, 3, "%.3f");
	transformSphere_.translate = { sphereTranslate[0],sphereTranslate[1],sphereTranslate[2] };

	ImGui::End();

	ImGui::Begin("Light");

	float directionalLightPosition[3] = { directionalLightData_->direction.x,directionalLightData_->direction.y,directionalLightData_->direction.z };
	ImGui::SliderFloat3("directionalLightPosition", directionalLightPosition, -1, 1, "%.3f");
	directionalLightData_->direction.x = directionalLightPosition[0];
	directionalLightData_->direction.y = directionalLightPosition[1];
	directionalLightData_->direction.z = directionalLightPosition[2];

	float directionalLightColor[3] = { directionalLightData_->color.x,directionalLightData_->color.y,directionalLightData_->color.w };
	ImGui::SliderFloat3("directionalLightColor", directionalLightColor, 0, 1, "%.3f");
	directionalLightData_->color.x = directionalLightColor[0];
	directionalLightData_->color.y = directionalLightColor[1];
	directionalLightData_->color.w = directionalLightColor[2];

	ImGui::End();
}

void MyEngine::Release()
{
	vertexResource_->Release();
	materialResource_->Release();
	wvpResource_->Release();

	for (int i = 0; i < kMaxTexture; i++)
	{
		if (CheckTextureIndex[i] == true)
		{
			textureResource_[i]->Release();
			intermediateResource_[i]->Release();
		}
	}

	vertexResourceSprite_->Release();
	transformationMatrixResourceSprite_->Release();
	materialResourceSprite_->Release();

	vertexResourceSphere_->Release();
	transformationMatrixResourceSphere_->Release();
	materialResourceSphere_->Release();

	directionalLightResource_->Release();
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

int MyEngine::LoadTexture(const std::string& filePath)
{
	int spriteIndex = 0;

	for (int i = 0; i < kMaxTexture; ++i)
	{
		if (CheckTextureIndex[i] == false)
		{
			spriteIndex = i;
			CheckTextureIndex[i] = true;
			break;
		}
	}

	if (spriteIndex < 0)
	{
		assert(false);
	}

	if (kMaxTexture < spriteIndex)
	{
		assert(false);
	}

	//Textureを読んで転送する
	DirectX::ScratchImage mipImages = OpenImage(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textureResource_[spriteIndex] = CreateTextureResource(dxCommon_->GetDevice(), metadata);
	intermediateResource_[spriteIndex] = UploadTextureData(textureResource_[spriteIndex], mipImages);

	//metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	//SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU_[spriteIndex] = GetCPUDescriptorHandle(dxCommon_->GetSrvDescriptorHeap(), descriptorSizeSRV_, spriteIndex);
	textureSrvHandleGPU_[spriteIndex] = GetGPUDescriptorHandle(dxCommon_->GetSrvDescriptorHeap(), descriptorSizeSRV_, spriteIndex);
	//先頭はImGuiが使っているので次のを使う
	textureSrvHandleCPU_[spriteIndex].ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU_[spriteIndex].ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの作成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource_[spriteIndex], &srvDesc, textureSrvHandleCPU_[spriteIndex]);

	return spriteIndex;
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
	materialResourceSprite_ = CreateBufferResource(sizeof(Material));
	transformationMatrixResourceSprite_ = CreateBufferResource(sizeof(TransformationMatrix));
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

void MyEngine::CreateVertexBufferViewSphere()
{
	vertexResourceSphere_ = CreateBufferResource(sizeof(VertexData) * 6 * kSubdivision_ * kSubdivision_);
	materialResourceSphere_ = CreateBufferResource(sizeof(Material));
	transformationMatrixResourceSphere_ = CreateBufferResource(sizeof(TransformationMatrix));
	vertexResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere_));

	vertexBufferViewSphere_.BufferLocation = vertexResourceSphere_->GetGPUVirtualAddress();

	vertexBufferViewSphere_.SizeInBytes = sizeof(VertexData) * 6 * kSubdivision_ * kSubdivision_;

	vertexBufferViewSphere_.StrideInBytes = sizeof(VertexData);
}

D3D12_CPU_DESCRIPTOR_HANDLE MyEngine::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE MyEngine::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}