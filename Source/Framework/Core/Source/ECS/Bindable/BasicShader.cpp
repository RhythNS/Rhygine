#include <ECS/Bindable/BasicShader.h>

BasicShader::BasicShader(LPCWSTR pixName, LPCWSTR vertName, std::vector<D3D11_INPUT_ELEMENT_DESC>* inputLayoutDesc) : worldBuffer({}), worldConstant(nullptr)
{
	CreateShaders(pixName, vertName, inputLayoutDesc);
	worldConstant = std::make_unique<ConstantVS<WorldPos>>(&worldBuffer, 0);
}

void BasicShader::Update(Drawer* drawer)
{
	worldBuffer.transform = GetPerspectiveMatrix(drawer);

	worldConstant->SetAndUpdate(&worldBuffer);
}

void BasicShader::InnerBind()
{
	worldConstant->Bind();
}
