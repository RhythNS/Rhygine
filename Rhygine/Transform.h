#pragma once
#include "RhyMath.h"
#include "Component.h"

#include <DirectXMath.h>

class Transform : public Component
{
	friend class Bindable;
public:
	void Init();
	Vec3 position = { 0.0f,0.0f,0.0f };
	Quat rotation = Quat(0.0f, 0.0f, 0.0f);
	Vec3 scale = { 1.0f,1.0f,1.0f };
private:
	DirectX::XMMATRIX perspectiveMatrix, worldMatrix, localMatrix;
	DirectX::XMMATRIX* GetWorldMatrix();
	DirectX::XMMATRIX* GetPerspectiveMatrix();
	DirectX::XMMATRIX* GetLocalMatrix();
};
