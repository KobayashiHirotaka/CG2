#pragma once
#include "MyMath.h"
#include "MyImGui.h"
#include <d3d12.h>
#include <wrl.h>

enum Lighting 
{
	NotDo = false,
	harfLambert = 1,
	Lambert = 2,
};

class Light
{
	struct DirectionalLight
	{
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

public:
	static Light* GetInstance();

	void Initialize();

	void ImGui(const char* Title);

	ID3D12Resource* GetDirectionalLight()const { return directionalLightResource_.Get(); }

private:
	/*Light() = default;
	~Light() = default;*/

	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
};

