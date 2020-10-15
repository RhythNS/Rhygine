#pragma once
#include "RhyWin.h"

#include <DirectXMath.h>

class Drawer;
class CombinedShader;

class Bindable
{
	friend class Drawer;
	friend class CombinedShader;
public:
	virtual void Bind() = 0;
	virtual void Init();
protected:
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwap();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext();
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetTarget();
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView();

	DirectX::XMMATRIX* GetWorldMatrix();
	DirectX::XMMATRIX* GetPerspectiveMatrix();
	DirectX::XMMATRIX* GetLocalMatrix();
	
	Drawer* GetDrawer();
private:
	Drawer* drawer;
};
