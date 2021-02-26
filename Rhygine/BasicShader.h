#pragma once
#include "CombinedShader.h"
#include "Updatable.h"
#include "ConstantBuffer.h"

#include <DirectXMath.h>
#include <memory>

/// <summary>
/// Basic shader which only needs the world position as a matrix.
/// </summary>
class BasicShader : public CombinedShader, public Updatable
{
public:
	/// <summary>
	/// Standard constructor.
	/// </summary>
	/// <param name="pixName">Path to the pixel shader.</param>
	/// <param name="vertName">Path to the vertex shader.</param>
	/// <param name="inputLayoutDesc">Input layout description.</param>
	BasicShader(LPCWSTR pixName, LPCWSTR vertName, std::vector<D3D11_INPUT_ELEMENT_DESC>* inputLayoutDesc);
	BasicShader() = delete;
	virtual ~BasicShader() { }

	struct WorldPos {
		DirectX::XMMATRIX transform;
	};

	void Update(Drawer* drawer);
protected:
	void InnerBind();
	void UpdateBuffer();
	std::unique_ptr<ConstantVS<WorldPos>> worldConstant;

	WorldPos worldBuffer;
};
