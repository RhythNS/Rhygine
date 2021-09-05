#include <ECS/Bindable/CombinedShader.h>
#include <ECS/Bindable/PixShader.h>
#include <ECS/Bindable/VertShader.h>
#include <ECS/Bindable/InputLayout.h>

void CombinedShader::Bind()
{
	InnerBind();
	pixShader->Bind();
	vertShader->Bind();
	inputLayout->Bind();
}

void CombinedShader::CreateShaders(LPCWSTR pixName, LPCWSTR vertName, std::vector<D3D11_INPUT_ELEMENT_DESC>* inputLayoutDesc)
{
	// Create shaders
	pixShader = std::make_unique<PixShader>(pixName);
	vertShader = std::make_unique<VertShader>(vertName);

	// Get the blob from the vert shader and create the input layout
	ID3DBlob* blob = vertShader->GetBlob();
	inputLayout = std::make_unique<InputLayout>(*inputLayoutDesc, blob);
}
 