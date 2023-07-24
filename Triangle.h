#pragma once
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")
#include "DirectXCommon.h"
#include "Vector4.h"

class MyEngine;

class Triangle
{
public:
	~Triangle() = default;

	// VertexResourceの生成
	void CreateVertexResource();

	// VertexBufferViewの生成
	void CreateVertexBufferView();

	// 初期化
	void Initialize(DirectXCommon* directXCommon, const Vector4& triangleData, const Vector4& triangleData2, const Vector4& triangleData3);

	// 三角形描画
	void Draw();

public:
	static const int kMaxTriangle = 11;
	DirectXCommon* directXCommon_;
	ID3D12Resource* vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	Vector4* vertexData_;
	Vector4 triangleData[kMaxTriangle][3];
};
