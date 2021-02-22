#pragma once
#include "Bindable.h"
#include "DrawerBindable.h"

#include <memory>
#include <vector>

class PixShader;
class VertShader;
class InputLayout;

/// <summary>
/// Represents a shader. Holds a pixel and vertex shader and an input layer bindable.
/// </summary>
class CombinedShader : public Bindable, public DrawerBindable
{
public:
	void Bind();

protected:
	/// <summary>
	/// Create the shaders and input layout.
	/// </summary>
	/// <param name="pixName">The path to the pixel shader.</param>
	/// <param name="vertName">The path to the vertex shader.</param>
	/// <param name="inputLayoutDesc">The input layout description.</param>
	void CreateShaders(LPCWSTR pixName, LPCWSTR vertName, std::vector<D3D11_INPUT_ELEMENT_DESC>* inputLayoutDesc);

	/// <summary>
	/// Should be used to bind all bindables which are on the shader.
	/// </summary>
	virtual void InnerBind() = 0;

	std::shared_ptr<PixShader> pixShader;
	std::shared_ptr<VertShader> vertShader;
	std::shared_ptr<InputLayout> inputLayout;
};
