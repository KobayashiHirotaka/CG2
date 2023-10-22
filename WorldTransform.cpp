#include "WorldTransform.h"

void WorldTransform::Initialize()
{
	matWorld = MakeIdentity4x4();
	CreateConstBuffer();
	Map();
	TransferMatrix();
}

void WorldTransform::CreateConstBuffer()
{
	constBuff = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataWorldTransform));
}

void WorldTransform::Map()
{
	constBuff.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
}

void WorldTransform::TransferMatrix()
{
	constMap->matWorld = matWorld;
}

void WorldTransform::UpdateMatrix()
{
	Matrix4x4 AffineMatrix = MakeAffineMatrix(scale, rotation, translation);
	matWorld = AffineMatrix;

	if (parent) 
	{
		matWorld = Multiply(matWorld, parent->matWorld);
	}

	TransferMatrix();
}

Vector3 WorldTransform::GetWorldPos()
{
	return { matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2] };
}
