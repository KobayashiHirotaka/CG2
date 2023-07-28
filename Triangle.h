#pragma once
#include"DirectXCommon.h"
#include"Vector4.h"

class MyEngine;

class Triangle
{
public:
	// 初期化
	void Initialize(DirectXCommon* directXCommon, const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material);

	// 三角形描画
	void Draw();

	void Finalize();

private:
	MyEngine* engine_;

	DirectXCommon* directXCommon_;

	Vector4* vertexData_;

	Vector4* materialData_;

	ID3D12Resource* vertexResource_;

	ID3D12Resource* materialResource_;
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

private:
	void SetVertex();

	void SettingColor();
};
