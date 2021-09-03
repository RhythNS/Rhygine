#pragma once
#include "RhyWin.h"

class Window;
class Gfx;
class Scene;
class Stage;

class Module
{
	friend class Window;

public:
	Module();
	Module(int overwriteExecutionOrder);
	virtual ~Module();

	int GetExecutionOrder();

protected:
	virtual void Setup();
	virtual void StartOfFramePreUpdate();
	virtual void PreDrawAfterUpdate();
	virtual void LateDraw();
	virtual void EndOfFramePreSleep();
	virtual void OnSceneChange(Scene* scene);

	Window* GetWindow();
	
	Gfx* GetGfx();
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwap();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext();
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetTarget();
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView();

	Scene* GetCurrentScene();
	Stage* GetCurrentStage();
	Stage* GetStage(Scene* scene);

	int executionOrder = 0;

private:
	Window* window = nullptr;
	Gfx* gfx = nullptr;
};
