#pragma once
#include "Vector4.h"
#include "Matrix4x4.h"
#include <stdint.h>

struct Material
{
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};
