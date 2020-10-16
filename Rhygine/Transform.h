#pragma once
#include "RhyMath.h"
#include "Component.h"

#include <DirectXMath.h>

class Transform : public Component
{
public:
	void Init();
	RhyM::Vec3 position = RhyM::Vec3(0.0f,0.0f,8.0f);
	RhyM::Quat rotation;
	RhyM::Vec3 scale = RhyM::Vec3(1.0f,1.0f,1.0f);

	RhyM::Vec3 GetForward();

	DirectX::XMMATRIX* GetWorldMatrix();
	DirectX::XMMATRIX* GetPerspectiveMatrix();
	DirectX::XMMATRIX* GetLocalMatrix();
private:
	DirectX::XMMATRIX perspectiveMatrix, worldMatrix, localMatrix;
};
