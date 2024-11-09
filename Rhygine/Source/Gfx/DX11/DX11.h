#pragma once
#include "Gfx.h"

#include <memory>
#include <vector>

#include "DX11Screen.h"
#include "Windows\RhyWindows.h"

namespace Rhygine
{
	class DX11 : public Gfx
	{
	public:
		DX11();
		~DX11();
		
		void OnWindowAdded(Window* t_window) override;
		void OnWindowResized(Window* t_window) override;
		void OnWindowRemoved(Window* t_window) override;

		void Draw() override;
	
	private:

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;

		std::vector<std::unique_ptr<DX11Screen>> m_screens;
	};
}
