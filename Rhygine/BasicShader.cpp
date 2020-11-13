#include "BasicShader.h"

BasicShader::BasicShader(LPCWSTR pixName, LPCWSTR vertName, std::vector<D3D11_INPUT_ELEMENT_DESC>* inputLayoutDesc) : worldBuffer({}), worldConstant(nullptr)
{
	CreateShaders(pixName, vertName, inputLayoutDesc);
}

void BasicShader::Init()
{
	UpdateBuffer();
	worldConstant = std::make_unique<ConstantVS<WorldPos>>(&worldBuffer, 0);
	InitBindable(worldConstant.get());
}

void BasicShader::Update()
{
	UpdateBuffer();

	worldConstant->SetAndUpdate(&worldBuffer);
}

void BasicShader::InnerBind()
{
	worldConstant->Bind();
}

void BasicShader::UpdateBuffer()
{
	worldBuffer.transform = GetPerspectiveMatrix();
}
