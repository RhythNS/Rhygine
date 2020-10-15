#pragma once
#include "CombinedShader.h"
#include "Updatable.h"
#include "ConstantBuffer.h"

#include <DirectXMath.h>
#include <memory>

class BasicShader : public CombinedShader, public Updatable
{
public:
	BasicShader(LPCWSTR pixName, LPCWSTR vertName, std::vector<D3D11_INPUT_ELEMENT_DESC>* inputLayoutDesc);

	struct WorldPos {
		DirectX::XMMATRIX transform;
	};

	void Init();
	void Update();
protected:
	void InnerBind();
	void UpdateBuffer();
	std::unique_ptr<ConstantVS<WorldPos>> worldConstant;

	WorldPos worldBuffer;
};
