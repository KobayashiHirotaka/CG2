#include "Triangle.h"
#include "WindowsApp.h"
#include "ConvertString.h"
#include <format>
#include <cassert>
#include "Vector4.h"
#include "DirectXCommon.h"

void Triangle::CreateVertexResource()
{
	HRESULT hr;
	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(Vector4) * 3; // リソースのサイズ。今回はVector4を3頂点分
	// バッファの場合はこれからは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	//ID3D12Resource* vertexResource{};
	hr = directXCommon_->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource_));
	assert(SUCCEEDED(hr));
}

void Triangle::CreateVertexBufferView()
{
	// 頂点バッファビューを生成する
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(Vector4) * 3;
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(Vector4);

	// 書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void Triangle::Initialize(DirectXCommon* directXCommon, const Vector4& triangleData, const Vector4& triangleData2, const Vector4& triangleData3)
{
	directXCommon_ = directXCommon;

	CreateVertexResource();

	CreateVertexBufferView();

	// 左下
	vertexData_[0] = triangleData;
	// 上
	vertexData_[1] = triangleData2;
	// 右下
	vertexData_[2] = triangleData3;
}

void Triangle::Draw()
{
	// コマンドを積む
	ID3D12GraphicsCommandList* commandList = directXCommon_->GetCommandList();

	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	// 形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 描画(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commandList->DrawInstanced(3, 1, 0, 0);
}