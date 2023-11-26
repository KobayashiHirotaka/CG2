#pragma once
#include "Sprite.h"
#include "Input.h"
#include "Enemy.h"
#include "TextureManager.h"
#include "ViewProjection.h"

class LockOn 
{
public:
	void Initialize();

	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	void Draw();
};


