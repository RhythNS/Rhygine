#pragma once
#include "Bindable.h"

#include <memory>
#include <vector>

class PixShader;
class VertShader;
class InputLayout;

class CombinedShader : public Bindable
{
public:
	void Bind();
protected:
	template<class T>
	inline void InitBindable(T* t)
	{
		t->drawer = drawer;
		t->Init();
	}
	void CreateShaders(LPCWSTR pixName, LPCWSTR vertName, std::vector<D3D11_INPUT_ELEMENT_DESC>* inputLayoutDesc);
	virtual void InnerBind() = 0;
	std::shared_ptr<PixShader> pixShader;
	std::shared_ptr<VertShader> vertShader;
	std::shared_ptr<InputLayout> inputLayout;
};
