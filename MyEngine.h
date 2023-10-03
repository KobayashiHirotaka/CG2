#pragma once
#include"DirectXCommon.h"
#include"MyMath.h"
#include"Matrix4x4.h"
#include"VertexData.h"
#include "Sphere.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "ModelData.h"
#include <fstream>
#include <sstream>
#include "externals/DirectXTex/DirectXTex.h"

class MyEngine
{
public:
	static MyEngine* GetInstance();

	void Initialize(DirectXCommon* dxCommon, int32_t kClientWidth, int32_t kClientHeight);

	void ImGui();

private:
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	HRESULT hr_;
	DirectXCommon* dxCommon_ = nullptr;
};
