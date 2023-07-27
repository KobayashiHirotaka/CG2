#pragma once
#include"DirectXCommon.h"
#include"Vector4.h"

class MyEngine;

class Triangle
{
public:
	// 初期化
	void Initialize(DirectXCommon* directXCommon, const Vector4& a, const Vector4& b, const Vector4& c);

	// 三角形描画
	void Draw();

	void Finalize();

public:
	MyEngine* engine_;

	DirectXCommon* directXCommon_;

	Vector4* vertexData_;

	ID3D12Resource* vertexResource_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

private:
	void SetVertex();
};
