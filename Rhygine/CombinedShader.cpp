#include "CombinedShader.h"
#include "PixShader.h"
#include "VertShader.h"
#include "InputLayout.h"

void CombinedShader::Bind()
{
	InnerBind();
	pixShader->Bind();
	vertShader->Bind();
	inputLayout->Bind();
}

void CombinedShader::CreateShaders(LPCWSTR pixName, LPCWSTR vertName, std::vector<D3D11_INPUT_ELEMENT_DESC>* inputLayoutDesc)
{
	pixShader = std::make_unique<PixShader>(pixName);
	InitBindable<PixShader>(pixShader.get());
	vertShader = std::make_unique<VertShader>(vertName);
	InitBindable<VertShader>(vertShader.get());

	ID3DBlob* blob = vertShader->GetBlob();

	inputLayout = std::make_unique<InputLayout>(*inputLayoutDesc, blob);
	InitBindable<InputLayout>(inputLayout.get());
}