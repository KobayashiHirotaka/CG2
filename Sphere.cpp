#include"Sphere.h"

void Sphere::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	light_ = Light::GetInstance();

	vertexResourceSphere_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * 4 * kSubdivision_ * kSubdivision_);
	materialResourceSphere_ = dxCommon_->CreateBufferResource(sizeof(Material));
	indexResourceSphere_ = dxCommon_->CreateBufferResource(sizeof(uint32_t) * 6 * kSubdivision_ * kSubdivision_);

	CreateVertexBufferViewSphere();
	CreateIndexBufferViewSphere();

	vertexResourceSphere_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere_));
	indexResourceSphere_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSphere_));

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
			uint32_t start = (latIndex * kSubdivision_ + lonIndex) * 4;
			uint32_t indexStart = (latIndex * kSubdivision_ + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;
			float u = float(lonIndex) / float(kSubdivision_);
			float v = 1.0f - float(latIndex) / float(kSubdivision_);

			vertexDataSphere_[start].position.x = cos(lat) * cos(lon) * cos(lon);
			vertexDataSphere_[start].position.y = sin(lat);
			vertexDataSphere_[start].position.w = cos(lat) * sin(lon);
			vertexDataSphere_[start].position.h = 1.0f;
			vertexDataSphere_[start].texcoord = { u,v + uvLength };
			vertexDataSphere_[start].normal.x = vertexDataSphere_[start].position.x;
			vertexDataSphere_[start].normal.y = vertexDataSphere_[start].position.y;
			vertexDataSphere_[start].normal.z = vertexDataSphere_[start].position.w;

			vertexDataSphere_[start + 1].position.x = cos(lat + kLatEvery) * cos(lon);
			vertexDataSphere_[start + 1].position.y = sin(lat + kLatEvery);
			vertexDataSphere_[start + 1].position.w = cos(lat + kLatEvery) * sin(lon);
			vertexDataSphere_[start + 1].position.h = 1.0f;
			vertexDataSphere_[start + 1].texcoord = { u,v };
			vertexDataSphere_[start + 1].normal.x = vertexDataSphere_[start + 1].position.x;
			vertexDataSphere_[start + 1].normal.y = vertexDataSphere_[start + 1].position.y;
			vertexDataSphere_[start + 1].normal.z = vertexDataSphere_[start + 1].position.w;

			vertexDataSphere_[start + 2].position.x = cos(lat) * cos(lon + kLonEvery);
			vertexDataSphere_[start + 2].position.y = sin(lat);
			vertexDataSphere_[start + 2].position.w = cos(lat) * sin(lon + kLonEvery);
			vertexDataSphere_[start + 2].position.h = 1.0f;
			vertexDataSphere_[start + 2].texcoord = { u + uvLength, v + uvLength };
			vertexDataSphere_[start + 2].normal.x = vertexDataSphere_[start + 2].position.x;
			vertexDataSphere_[start + 2].normal.y = vertexDataSphere_[start + 2].position.y;
			vertexDataSphere_[start + 2].normal.z = vertexDataSphere_[start + 2].position.w;

			vertexDataSphere_[start + 3].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
			vertexDataSphere_[start + 3].position.y = sin(lat + kLatEvery);
			vertexDataSphere_[start + 3].position.w = cos(lat + kLatEvery) * sin(lon + kLonEvery);
			vertexDataSphere_[start + 3].position.h = 1.0f;
			vertexDataSphere_[start + 3].texcoord = { u + uvLength,v };
			vertexDataSphere_[start + 3].normal.x = vertexDataSphere_[start + 3].position.x;
			vertexDataSphere_[start + 3].normal.y = vertexDataSphere_[start + 3].position.y;
			vertexDataSphere_[start + 3].normal.z = vertexDataSphere_[start + 3].position.w;

			/*vertexDataSphere_[start + 4].position.x = cos(lat) * cos(lon + kLonEvery) + sphere.center.x;
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
			vertexDataSphere_[start + 5].normal.z = vertexDataSphere_[start + 5].position.w;*/

			indexDataSphere_[indexStart] = start;
			indexDataSphere_[indexStart + 1] = start + 1;
			indexDataSphere_[indexStart + 2] = start + 2;

			indexDataSphere_[indexStart + 3] = start + 1;
			indexDataSphere_[indexStart + 4] = start + 3;
			indexDataSphere_[indexStart + 5] = start + 2;
		}
	}
}

void Sphere::Draw(const WorldTransform& transform, const ViewProjection& viewProjection, const uint32_t& TextureHandle)
{
	materialResourceSphere_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSphere_));

	//ライティング
	materialDataSphere_->color = color_;
	materialDataSphere_->enableLighting = lightFlag;
	materialDataSphere_->uvTransform = MakeIdentity4x4();

	dxCommon_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点
	dxCommon_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSphere_);
	dxCommon_->GetcommandList()->IASetIndexBuffer(&indexBufferViewSphere_);

	//WorldTransform
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transform.constBuff.Get()->GetGPUVirtualAddress());

	//ViewProjection
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(4, viewProjection.constBuff->GetGPUVirtualAddress());

	//色用のCBufferの場所を特定
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSphere_.Get()->GetGPUVirtualAddress());

	//テクスチャ
	dxCommon_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(TextureHandle));

	//Light
	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(3, light_->GetDirectionalLight()->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->DrawIndexedInstanced(kSubdivision_ * kSubdivision_ * 6, 1, 0, 0, 0);
}

void Sphere::CreateVertexBufferViewSphere()
{
	//リソースの先頭のアドレス
	vertexBufferViewSphere_.BufferLocation = vertexResourceSphere_.Get()->GetGPUVirtualAddress();
	//使用する頂点サイズ
	vertexBufferViewSphere_.SizeInBytes = sizeof(VertexData) * 4 * kSubdivision_ * kSubdivision_;
	//1頂点あたりのアドレス
	vertexBufferViewSphere_.StrideInBytes = sizeof(VertexData);
}
void Sphere::CreateIndexBufferViewSphere()
{
	//リソース先頭アドレス
	indexBufferViewSphere_.BufferLocation = indexResourceSphere_.Get()->GetGPUVirtualAddress();
	//使用するインデックスサイズ
	indexBufferViewSphere_.SizeInBytes = sizeof(uint32_t) * 6 * kSubdivision_ * kSubdivision_;
	//インデックスはuint32_t
	indexBufferViewSphere_.Format = DXGI_FORMAT_R32_UINT;
}
