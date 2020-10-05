#pragma once
#include <DirectXMath.h>

#include "RhyMath.h"

class Transform
{
	friend class Gameobject;
public:
	Vec3 position = { 0.0f,0.0f,0.0f };
	Quat rotation = Quat(0.0f, 0.0f, 0.0f);
	Vec3 scale = { 1.0f,1.0f,1.0f };
private:
	struct TransformBuffer {
		DirectX::XMMATRIX transform;
	};
	TransformBuffer buffer;

	TransformBuffer* GetCurrentTransform();
};
