#pragma once
#include <RhyWin.h>

#include <DirectXMath.h>

class Drawer;
class CombinedShader;

/// <summary>
/// Bindables represent resources that are needed to render an object.
/// Bindables exist inside the drawer component. When the drawer is drawing,
/// every bindable is first updated, when they implement the updatable interface,
/// and then binded. Lastly they are unbound, when they implement the unbindable interface.
/// </summary>
class Bindable
{
	friend class Drawer;
	friend class CombinedShader;
public:
	virtual ~Bindable() { }

	/// <summary>
	/// Readys the bindable to be drawn to the screen.
	/// </summary>
	virtual void Bind() = 0;
protected:
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwap();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext();
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetTarget();
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView();

	DirectX::XMMATRIX GetWorldMatrix(Drawer* drawer);
	DirectX::XMMATRIX GetPerspectiveMatrix(Drawer* drawer);
	DirectX::XMMATRIX GetLocalMatrix(Drawer* drawer);
};
