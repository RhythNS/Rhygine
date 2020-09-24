#pragma once
#include <DirectXMath.h>

class Window;

class TestCamera
{
	struct Vector {
		float x, y, z;
	};

public:
	TestCamera();
	TestCamera(Vector position, Vector rotation);

	void HandleInput(Window* window);

	Vector position, rotation;
	DirectX::XMMATRIX GetMatrix();
};

