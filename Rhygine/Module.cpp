#include "Module.h"
#include "Window.h"
#include "Gfx.h"
#include "Scene.h"
#include "Stage.h"

Module::Module()
{
}

Module::Module(int overwriteExecutionOrder) : executionOrder(overwriteExecutionOrder)
{
}

Module::~Module()
{
}

int Module::GetExecutionOrder()
{
    return executionOrder;
}

void Module::Setup()
{
}

void Module::StartOfFramePreUpdate()
{
}

void Module::PreDrawAfterUpdate()
{
}

void Module::LateDraw()
{
}

void Module::EndOfFramePreSleep()
{
}

void Module::OnSceneChange(Scene* scene)
{
}

Window* Module::GetWindow()
{
	return window;
}

Gfx* Module::GetGfx()
{
	return gfx;
}

Microsoft::WRL::ComPtr<ID3D11Device> Module::GetDevice()
{
    return gfx->device;
}

Microsoft::WRL::ComPtr<IDXGISwapChain> Module::GetSwap()
{
    return gfx->swap;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> Module::GetContext()
{
    return gfx->context;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Module::GetTarget()
{
    return gfx->target;
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> Module::GetDepthStencilView()
{
    return gfx->depthStencilView;
}

Scene* Module::GetCurrentScene()
{
    return window->GetCurrentScene();
}

Stage* Module::GetCurrentStage()
{
    return window->GetCurrentScene()->stage.get();
}

Stage* Module::GetStage(Scene* scene)
{
    return scene->stage.get();
}
