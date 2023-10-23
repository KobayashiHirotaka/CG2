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

	if (parent_) 
	{
		matWorld = Multiply(matWorld, parent_->matWorld);
	}

	TransferMatrix();
}

void WorldTransform::SetParent(const WorldTransform* parent)
{
	parent_ = parent;
	translation = Subtract(translation, parent->translation);
}

void WorldTransform::UnsetParent()
{
	translation = { Vector3{matWorld.m[3][0],matWorld.m[3][1],matWorld.m[3][2]} };

	rotation.x = std::atan2(matWorld.m[2][1], matWorld.m[2][2]);
	rotation.y = -std::asin(matWorld.m[2][0]);
	rotation.z = std::atan2(matWorld.m[1][0], matWorld.m[0][0]);

	scale.x = Length(Vector3{ matWorld.m[0][0], matWorld.m[0][1], matWorld.m[0][2] });
	scale.y = Length(Vector3{ matWorld.m[1][0], matWorld.m[1][1], matWorld.m[1][2] });
	scale.z = Length(Vector3{ matWorld.m[2][0], matWorld.m[2][1], matWorld.m[2][2] });
	
	parent_ = nullptr;
}
