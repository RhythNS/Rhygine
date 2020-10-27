#pragma once
#include "RhyWin.h"

class Window;
class Bindable;

/// <summary>
/// Represents the conncetion to all dxd classes.
/// </summary>
class Gfx
{
	friend class Window;
	friend class Bindable;
public:
	Gfx() = delete;
	Gfx(Window* window);

	/// <summary>
	/// Singleton to get the Gfx instance.
	/// </summary>
	static Gfx* GetInstance();

	/// <summary>
	/// Called before gameobjects are started to be drawn.
	/// </summary>
	void BeginDraw();
	/// <summary>
	/// Draws an indexed object onto the screen.
	/// </summary>
	/// <param name="indexCount">The amount of indicies.</param>
	void DrawIndexed(UINT* indexCount);
	/// <summary>
	/// Draws an object onto the screen.
	/// </summary>
	/// <param name="vertexCount">The amount if verticies.</param>
	void Draw(UINT* vertexCount);
	/// <summary>
	/// Called after all gameobjects have been drawn.
	/// </summary>
	void EndDraw();
private:
	Window* window;

	static Gfx* instance;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
};

