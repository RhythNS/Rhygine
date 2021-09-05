#pragma once
#include <RhyWin.h>

#include <DirectXMath.h>

class Window;
class Bindable;
class Module;

/// <summary>
/// Represents the conncetion to all d3d classes.
/// </summary>
class Gfx
{
	friend class Window;
	friend class Bindable;
	friend class Module;
public:
	Gfx() = delete;
	Gfx(Window* window, int refreshRate, bool vsync);
	~Gfx();

	/// <summary>
	/// Singleton to get the Gfx instance.
	/// </summary>
	static Gfx* GetInstance();

	/// <summary>
	/// Clears the depth buffer.
	/// </summary>
	void ClearDepth();
	/// <summary>
	/// Called before gameobjects are started to be drawn.
	/// </summary>
	void BeginDraw();
	/// <summary>
	/// Draws an indexed object onto the screen.
	/// </summary>
	/// <param name="indexCount">The amount of indicies.</param>
	void DrawIndexed(UINT indexCount, UINT startLocation = 0, INT baseVertexLocation = 0);
	/// <summary>
	/// Draws an object onto the screen.
	/// </summary>
	/// <param name="vertexCount">The amount if verticies.</param>
	void Draw(UINT vertexCount, UINT startVertexLocation = 0);
	/// <summary>
	/// Called after all gameobjects have been drawn.
	/// </summary>
	void EndDraw();
	/// <summary>
	/// Resets the pipeline to use the default stencil state.
	/// </summary>
	void SetDefaultStencilState();

	/// <summary>
	/// Gets the lh perspective matrix. Near and far values can be changed via
	/// the Gfx fields nearZ and farZ.
	/// </summary>
	DirectX::XMMATRIX GetPerspectiveMatrix();

	bool vsync = true;
	float nearZ = 0.5f;
	float farZ = 500.0f;
private:
	/// <summary>
	/// Should be called after the win32 window was resized.
	/// </summary>
	/// <param name="newWidth">The new width in pixel.</param>
	/// <param name="newHeight">The new height in pixel.</param>
	void OnResize(int newWidth, int newHeight);

	/// <summary>
	/// Creates the render target and depth buffer.
	/// </summary>
	inline void CreateTargetAndDepth();

	Window* window;
	static Gfx* instance;
	int refreshRate;
	int width, height;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> defaultStencilState;
};

