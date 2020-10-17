#pragma once
#include "Component.h"

#include <DirectXMath.h>

class Transform;
class Stage;

class Camera : public Component
{
	friend class Stage;
public:
	void Init();
	Transform* GetTransform();

	DirectX::XMMATRIX* GetMatrix();

private:
	void UpdateMatrix();

	Transform* transform;
	DirectX::XMMATRIX matrix;
};

